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

#include "QiliSocket.h"

#include "QiliProtocol.h"
#include "QiliGlobal.h"
#include "Utility.h"

#include <QAbstractSocket>
#include <QNetworkRequest>
#include <QWebSocket>
#include <QDateTime>

using namespace Utility::JSON;


QiliSocket::QiliSocket(QObject *parent)
    : QObject(parent)
{
    mSocket = new QWebSocket(QString(), QWebSocketProtocol::Version13);
    // mSocket->ignoreSslErrors();
    QObject::connect(mSocket, &QWebSocket::connected, this, &QiliSocket::authenticate);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QObject::connect(mSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred), this, &QiliSocket::handleError);
#else
    QObject::connect(mSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &QiliSocket::handleError);
#endif
    QObject::connect(mSocket, &QWebSocket::binaryMessageReceived, this, &QiliSocket::receivedBinaryMessage);

    mTimer = new QTimer(this);
    mTimer->setInterval(20000);
    QObject::connect(mTimer, &QTimer::timeout, this, &QiliSocket::heartbeat);
}

QiliSocket::~QiliSocket()
{
    // delete mSocket;
    // delete mTimer;
}

void QiliSocket::authenticate()
{
    qDebug() << "Connected, Now authicating with: " << mAuth;
    QByteArray data = mAuth.toUtf8();
    mSocket->sendBinaryMessage(Codec::encode(OP_AUTH, data, BodyType::PLAIN_AUTH));
}

void QiliSocket::heartbeat()
{
    qDebug() << "heartbeat: time = " << QDateTime::currentDateTime()
             << ",state = " << mSocket->state();
    // It seems that the client side shoud not check heartbeat status
    if (Qili::Released) { // enabled ONLY if in released binary
        // if can't receive heartbeat reply in 3 cycle, we should close the connection
        qint64 now = QDateTime::currentSecsSinceEpoch();
        if (!mSocket->isValid() || mLastHBAt > 0 && now - mLastHBAt > 60) {
            emit errorOccured("Socket goes invalid");
            return;
        }
    }
    mSocket->sendBinaryMessage(Codec::encode(OP_HEARTBEAT, BodyType::PLAIN_DATA));
}

void QiliSocket::receivedHeartbeatReply(const QJsonObject &pack)
{
    mLastHBAt = QDateTime::currentSecsSinceEpoch();
    qDebug() << "receivedHeartbeatReply: " << pack;
    int count = pack / "watchers" >> JInt;
    emit watchersChanged(count);
}

void QiliSocket::receivedAuthReply(const QJsonObject &pack)
{
    int code = pack / "code" >> JInt;
    qDebug() << "receivedAuthReply: pack = " << pack;
    qDebug() << "receivedAuthReply: code = " << code;
    if (code == 0) {
        // heartbeat();
        mTimer->start();
        emit authenticated();
    } else {
        qDebug() << "receivedAuthReply: failed = " << pack;
        emit errorOccured("Auth failed");
    }
}

void QiliSocket::receivedSmsReply(const QJsonObject &pack)
{
    qDebug() << "receivedSmsReply: " << pack;
    emit subtitleReceived(pack);
}


void QiliSocket::handleError(QAbstractSocket::SocketError error)
{
    qCritical() << "Error Occured: code = " << error;
    qCritical() << "Error Occured: msg = " << mSocket->errorString();
    emit errorOccured(mSocket->errorString());
}

void QiliSocket::receivedBinaryMessage(const QByteArray &bytes)
{
    qDebug() << "Binary Received: size = " << bytes.size();
    QList<QJsonObject> packs = Codec::decode(bytes);

    for (const auto &pack : packs) {
        int operation = pack / "op" >> JInt;
        switch (operation) {
        case OP_HEARTBEAT_REPLY:
            receivedHeartbeatReply(pack);
            break;
        case OP_AUTH_REPLY:
            receivedAuthReply(pack);
            break;
        case OP_SEND_SMS_REPLY:
            receivedSmsReply(pack);
            break;
        default:
            qWarning() << "Received Unknown Pack: " << pack;
        }
    }
}

void QiliSocket::open()
{
    QNetworkRequest request(mUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, Qili::UserAgent);
    qDebug() << "Connecting to: " << mUrl;
    mSocket->open(mUrl);
}

void QiliSocket::close()
{
    mTimer->stop();
    mSocket->abort();
}

void QiliSocket::setAuth(const QString &auth)
{
    mAuth = auth;
}
void QiliSocket::setUrl(const QString &url)
{
    mUrl = url;
}
