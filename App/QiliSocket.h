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

#ifndef QILISOCKET_H
#define QILISOCKET_H

#include "QiliAppGlobal.h"
#include "QiliProtocol.h"

#include <QObject>
#include <QWebSocket>
#include <QTimer>

class QILI_APP_EXPORT QiliSocket : public QObject
{
    Q_OBJECT
public:
    explicit QiliSocket(QObject *parent = nullptr);
    ~QiliSocket();

signals:
    void authenticated();
    void errorOccured(const QString &error);
    void subtitleReceived(const QJsonObject &sub);
    void watchersChanged(int count);

public slots:
    void open();
    void setAuth(const QString &auth);
    void setUrl(const QString &url);
    void close();

private slots:
    void authenticate();
    void heartbeat();

    void handleError(QAbstractSocket::SocketError error);
    void receivedBinaryMessage(const QByteArray &bytes);
    void receivedHeartbeatReply(const QJsonObject &pack);
    void receivedAuthReply(const QJsonObject &pack);
    void receivedSmsReply(const QJsonObject &pack);

private:
    QString     mAuth;
    QString     mUrl;

    QWebSocket *mSocket;

    QTimer     *mTimer;
    qint64      mLastHBAt{0};
};

#endif // QILISOCKET_H
