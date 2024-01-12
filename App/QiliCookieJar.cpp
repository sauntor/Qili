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

#include "QiliCookieJar.h"

#include <QNetworkCookie>
#include <QDebug>

QiliCookieJar::QiliCookieJar(QObject *parent)
    : QNetworkCookieJar(parent)
{}

QiliCookieJar::~QiliCookieJar()
{}

QList<QNetworkCookie> QiliCookieJar::allCookies() const
{
    return QNetworkCookieJar::allCookies();
}

bool QiliCookieJar::insertCookie(const QNetworkCookie &cookie)
{
    bool inserted = QNetworkCookieJar::insertCookie(cookie);
    if (inserted) {
        emit cookieInserted(cookie);
    }
    return inserted;
}

bool QiliCookieJar::updateCookie(const QNetworkCookie &cookie)
{
    bool updated = QNetworkCookieJar::updateCookie(cookie);
    if (updated) {
        emit cookieUpdated(cookie);
    }
    return updated;
}

bool QiliCookieJar::deleteCookie(const QNetworkCookie &cookie)
{
    bool deleted = QNetworkCookieJar::deleteCookie(cookie);
    if (deleted) {
        emit cookieDeleted(cookie);
    }
    return deleted;
}
