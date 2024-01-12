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

#ifndef QILIPROTOCOL_H
#define QILIPROTOCOL_H

#include "QiliAppGlobal.h"

#include <QByteArray>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QIODevice>
#include <QList>
#include <QString>

enum Opcode {
    OP_HEARTBEAT = 2, // 客户端发送的心跳包(30秒发送一次)
    OP_HEARTBEAT_REPLY = 3,	//服务器收到心跳包的回复
    OP_SEND_SMS_REPLY = 5, //服务器推送的弹幕消息包
    OP_AUTH = 7, //客户端发送的鉴权包(客户端发送的第一个包)
    OP_AUTH_REPLY = 8 //服务器收到鉴权包后的回复
};

enum BodyType {
    PLAIN_DATA = 0,
    PLAIN_AUTH = 1,
    ZLIB_DATA = 2,
    BROTLI_DATA = 3
};

namespace Commands
{
    // 直播间内用户排名
    constexpr const char * const ONLINE_RANK_V2 = "ONLINE_RANK_V2";
    // 弹幕
    constexpr const char * const DANMU_MSG = "DANMU_MSG";
    // 几人看过
    constexpr const char * const WATCHED_CHANGE = "WATCHED_CHANGE";

};

class QILI_APP_EXPORT Codec
{
public:

    static QByteArray encode(int opcode, BodyType bodyType = BodyType::PLAIN_DATA)
    {
        return encode(opcode, QByteArray());
    }

    static QByteArray encode(int opcode, const QJsonObject &data, BodyType bodyType = BodyType::PLAIN_DATA)
    {
        QByteArray body = QJsonDocument(data).toJson(QJsonDocument::Compact);
        return encode(opcode, body, bodyType);
    }

    static QByteArray encode(int opcode, const QByteArray &body, BodyType bodyType = BodyType::PLAIN_DATA);

    static QList<QJsonObject> decode(const QByteArray &bytes);

private:
    Codec() = delete;

    static void decode(const QByteArray &bytes, QList<QJsonObject> &packs);
    static void decode(QDataStream &io, QList<QJsonObject> &packs);
};

#endif // QILIPROTOCOL_H
