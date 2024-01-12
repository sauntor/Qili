/* Copyright (C) 2024  Sauntor <sauntor@live.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "QiliProtocol.h"

#include <brotli/decode.h>
#include <QDebug>

// for logging purpose only
Q_DECLARE_FLAGS(BrotliDecoderResults, BrotliDecoderResult)
Q_DECLARE_OPERATORS_FOR_FLAGS(BrotliDecoderResults)

const auto BROTLI_BUFFER_SIZE = 256;
QByteArray decode_brotli_compressed(const QByteArray &compressed)
{
    auto *decoder = BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);
    if (decoder == nullptr) {
        qCritical() << "Can't create brotli decoder instance ";
        return QByteArray();
    }
    QByteArray decompressed;

    size_t available_in = compressed.size();
    const uint8_t *next_in = reinterpret_cast<const uint8_t *>(compressed.constData());

    size_t available_out;
    uint8_t *next_out;

    BrotliDecoderResult result;
    std::array<uint8_t, BROTLI_BUFFER_SIZE> buffer;
    while (true) {
        available_out = buffer.size();
        next_out = buffer.data();
        result = BrotliDecoderDecompressStream(decoder, &available_in, &next_in, &available_out, &next_out, nullptr);
        decompressed.append(reinterpret_cast<const char *>(buffer.data()), buffer.size() - available_out);
        if (result == BROTLI_DECODER_RESULT_ERROR ||
            result == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT ||
            available_in == 0 && result == BROTLI_DECODER_RESULT_SUCCESS) {
            break;
        }
    }
    if (result != BROTLI_DECODER_RESULT_SUCCESS) {
        qWarning() << "BROTLI_DECODER FAILED: result = " << result;
        qWarning() << "BROTLI_DECODER FAILED: compressed = " << compressed;
    }

    BrotliDecoderDestroyInstance(decoder);
    return decompressed;
}

QByteArray Codec::encode(int opcode, const QByteArray &body, BodyType bodyType)
{
    QByteArray bytes;
    QDataStream io(&bytes, QIODevice::WriteOnly);
    io.setByteOrder(QDataStream::BigEndian);

    qint32 bodySize = body.size();

    qint16 header = 16;
    qint32 length = header + bodySize;
    qint16 type = bodyType;
    qint32 operation = opcode;
    qint32 sequence = 0;

    io << length;
    io << header;
    io << type;
    io << operation;
    io << sequence;
    io.writeRawData(body.constData(), bodySize);
    return bytes;
}

void Codec::decode(const QByteArray &bytes, QList<QJsonObject> &packs)
{
    QDataStream io((QByteArray *) &bytes, QIODevice::ReadOnly);
    io.setByteOrder(QDataStream::BigEndian);
    decode(io, packs);
}

QList<QJsonObject> Codec::decode(const QByteArray &bytes)
{
    QList<QJsonObject> packs;
    decode(bytes, packs);
    return packs;
}

void Codec::decode(QDataStream &io, QList<QJsonObject> &packs)
{
    qint32 length = 0;
    qint16 header = 0;
    qint16 type = 0;
    qint32 operation = 0;
    qint32 sequence = 0;

    io >> length;
    io >> header;
    io >> type;
    io >> operation;
    io >> sequence;

    qDebug() << "Received { length = " << length
             << ", header = " << header
             << ", type = " << type
             << ", operation = " << operation
             << ", sequence = " << sequence
             << "}";

    if (operation == OP_HEARTBEAT_REPLY) {
        qint32 watchers = 0;
        int remaining = io.device()->bytesAvailable();
        if (remaining >= 4) {
            io >> watchers;
            remaining -= 4;
        }
        qInfo() << "Received Heartbeat: " << watchers;
        packs << QJsonObject {
            {"op", OP_HEARTBEAT_REPLY},
            {"cmd", "QILI_HEARTBEAT_REPLY"},
            {"watchers", watchers}
        };

        if (remaining > 0) {
            qWarning() << "HeartBeat: " << remaining << " bytes after body";
            io.skipRawData(remaining);
        }
        return;
    }

    int bodySize = length - header;
    QByteArray raw;
    raw.resize(bodySize);
    io.readRawData(raw.data(), raw.size());
    QByteArray decompressed;
    switch(type) {
    case BROTLI_DATA:
        qDebug() << "Decode Compressed BROTLI Body...";
        decompressed = decode_brotli_compressed(raw);
        qDebug() << "Decode Compressed BROTLI Recursively: START";
        decode(decompressed, packs);
        qDebug() << "Decode Compressed BROTLI Recursively: END";
        return;
    case ZLIB_DATA:
        qDebug() << "Decode Compressed ZLIB Body...";
        decompressed = qUncompress(raw);
        qDebug() << "Decode Compressed ZLIB Recursize: START";
        decode(decompressed, packs);
        qDebug() << "Decode Compressed ZLIB Recursize: END";
        return;
    case PLAIN_DATA:
    case PLAIN_AUTH:
            qDebug() << "Received NONE compressed data";
            decompressed = raw;
        break;
    default:
        qWarning() << "Unknow body: type = " << type;
        qWarning() << "Unknow body: data = " << raw;
        return;
    }

    QJsonParseError state;
    // qDebug() << "Body Bytes = " << decompressed;
    QJsonDocument json = QJsonDocument::fromJson(decompressed, &state);
    if (state.error != QJsonParseError::NoError) {
        qWarning() << "Can't parse body json: " << state.errorString();
        return;
    }
    QJsonObject body = json.object();
    // qDebug() << "Body: " << QJsonDocument(body).toJson(QJsonDocument::Indented);
    body["op"] = operation;
    packs << body;

    if (!io.atEnd()) {
        qDebug() << "More bytes avalilable: " << io.device()->bytesAvailable() << ", sequencially decoding...";
        decode(io, packs);
    }
}
