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

#ifndef QILIHTTP_H
#define QILIHTTP_H

#include "QiliAppGlobal.h"
#include "QiliCookieJar.h"

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDateTime>
#include <QUuid>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QSharedPointer>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QStringList>


using HttpReply = QSharedPointer<QNetworkReply>;

class QILI_APP_EXPORT QiliHttp : QObject
{
    Q_OBJECT

    using Parameters = std::initializer_list<QPair<QString, QVariant>>;

public:
    explicit QiliHttp(QObject *parent = nullptr);
    ~QiliHttp();

    static QNetworkRequest newRequest(const QString &url, Parameters parameters = {}, int timeout = -1);

    QiliCookieJar *cookieJar() const;
    QByteArray storabelCookies() const;

    void restoreCookies(const QByteArray &data = QByteArray());

    int buid();
    QDateTime expires();
    HttpReply bapi(const QString &url, const QByteArray &data, int timeout = -1);

    HttpReply get(const QString &url, Parameters parameters = {}, int timeout = -1);

    QNetworkReply *getAsync(const QString &url, Parameters parameters = {}, int timeout = -1);

private:
    QNetworkAccessManager *networkAccessManager;
    QiliCookieJar         *mCookieJar;
};

#endif // QILIHTTP_H
