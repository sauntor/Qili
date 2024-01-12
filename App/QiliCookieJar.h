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

#ifndef QILICOOKIEJAR_H
#define QILICOOKIEJAR_H

#include "QiliAppGlobal.h"

#include <QObject>
#include <QNetworkCookieJar>

class QILI_APP_EXPORT QiliCookieJar : public QNetworkCookieJar
{
    Q_OBJECT

public:
    explicit QiliCookieJar(QObject *parent = nullptr);
    ~QiliCookieJar();

    QList<QNetworkCookie> allCookies() const;

    virtual bool insertCookie(const QNetworkCookie &cookie) override;
    virtual bool updateCookie(const QNetworkCookie &cookie) override;
    virtual bool deleteCookie(const QNetworkCookie &cookie) override;

signals:
    void cookieInserted(QNetworkCookie cookie);
    void cookieUpdated(QNetworkCookie cookie);
    void cookieDeleted(QNetworkCookie cookie);
};

#endif // QILICOOKIEJAR_H
