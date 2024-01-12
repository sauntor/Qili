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
#include "QiliConnection.h"

#include "QiliGlobal.h"
#include "Utility.h"

#include <QUuid>

using namespace Utility::JSON;

QiliConnection::QiliConnection(QiliHttp *http, QObject *parent)
    : QObject(parent),
      mHttp(http)
{
    mSocket = new QiliSocket(this);

    QObject::connect(mSocket, &QiliSocket::authenticated, this, &QiliConnection::authenticated);
    QObject::connect(mSocket, &QiliSocket::watchersChanged, this, &QiliConnection::onWatchersCount);
    QObject::connect(mSocket, &QiliSocket::subtitleReceived, this, &QiliConnection::subtitleReceived);

    QObject::connect(mSocket, &QiliSocket::errorOccured, this, &QiliConnection::onSocketError);
}

void QiliConnection::setRoom(int room)
{
    mRoom = room;
}

void QiliConnection::connect()
{
    HttpReply reply = mHttp->get(Qili::Uris::GetDanmuInfo, {{"id", mRoom}});
    QJsonObject json = reply->readAll() >> JObject;
    QJsonObject data = json / "data" >> JObject;
    QString token = data / "token" >> JString;
    QJsonArray array = data.value("host_list").toArray();
    auto item = array.first() >> JObject;
    auto wslink = QString("wss://%1:%2/sub").arg(item / "host" >> JString).arg(item / "wss_port" >> JInt);
    QJsonObject auth {
        {"uid", mHttp->buid()},
        {"roomid", mRoom},
        {"protover", 3},
        {"platform", "web"},
        {"type", 2},
        {"key", token}
    };
    qDebug() << "WebSocket Auth Body: " << auth;
    mSocket->setAuth(auth >> JString);
    mSocket->setUrl(wslink);
    mSocket->open();
}

void QiliConnection::disconnect()
{
    mSocket->close();
}

void QiliConnection::onWatchersCount(int watchers)
{
    if (watchers != mWatchers) {
        mWatchers = watchers;
        emit watchersChanged(watchers);
    }
}

void QiliConnection::onSocketError(const QString &errorString)
{
    emit errorOccured(SocketError, errorString);
}

