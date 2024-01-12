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
#ifndef QILICONNECTION_H
#define QILICONNECTION_H

#include "QiliAppGlobal.h"
#include "QiliSocket.h"
#include "QiliHttp.h"

#include <QObject>
#include <QJsonObject>

class QILI_APP_EXPORT QiliConnection : public QObject
{
    Q_OBJECT

public:
    enum Error { StartFailed, EndFailed, SocketError };

    explicit QiliConnection(QiliHttp *http, QObject *parent = nullptr);

signals:
    void authenticated();
    void watchersChanged(int watchers);
    void subtitleReceived(const QJsonObject &subtitle);
    void errorOccured(Error error, const QString &message);

public slots:
    void setRoom(int room);
    void connect();
    void disconnect();

private slots:
    void onWatchersCount(int watchers);
    // void onSubtitleReceived(const QJsonObject &subtitle);
    // void onAuthenticated();
    void onSocketError(const QString &errorString);

private:
    int             mRoom{0};
    int             mWatchers{0};
    QiliSocket     *mSocket;
    QiliHttp       *mHttp;

};

#endif // QILICONNECTION_H
