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

#include "QiliHttp.h"

#include "QiliGlobal.h"

#include <QIODevice>
#include <QDataStream>
#include <QNetworkCookie>
#include <QUrlQuery>
#include <QByteArray>


QiliHttp::QiliHttp(QObject *parent)
{
    networkAccessManager = new QNetworkAccessManager();
    mCookieJar = new QiliCookieJar();
    networkAccessManager->setCookieJar(mCookieJar);
}

QiliHttp::~QiliHttp()
{
    delete networkAccessManager;
}

QNetworkRequest QiliHttp::newRequest(const QString &url, Parameters parameters, int timeout)
{
    QUrl urlWithQueryString(url);
    QUrlQuery query(url);
    for (const auto &parameter : parameters) {
        query.addQueryItem(parameter.first, parameter.second.toString());
    }
    urlWithQueryString.setQuery(query);
    QNetworkRequest request(urlWithQueryString);
    request.setHeader(QNetworkRequest::UserAgentHeader, Qili::UserAgent);
    if (timeout == 0) {
        // Use the default timeout value: 30s
        request.setTransferTimeout(QNetworkRequest::DefaultTransferTimeoutConstant);
    }
    if (timeout > 0) {
        request.setTransferTimeout(timeout);
    }
    return request;
}

QiliCookieJar *QiliHttp::cookieJar() const
{
    return mCookieJar;
}

QDataStream &operator<<(QDataStream &stream, const QNetworkCookie &cookie)
{
    stream << cookie.toRawForm();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QNetworkCookie &cookie)
{
    QByteArray bytes;
    stream >> bytes;
    auto cookies = QNetworkCookie::parseCookies(bytes);
    cookie = cookies.first();
    return stream;
}

QByteArray QiliHttp::storabelCookies() const
{
    QByteArray bytes;
    QDataStream io(&bytes, QIODevice::WriteOnly);
    auto cookies = mCookieJar->allCookies();
    io << cookies;
    return bytes;
}

void QiliHttp::restoreCookies(const QByteArray &data)
{
    if (data.isEmpty()) {
        auto cookies = mCookieJar->allCookies();
        for (const auto &cookie : std::as_const(cookies)) {
            mCookieJar->deleteCookie(cookie);
        }
        return;
    }
    QDataStream io((QByteArray *) &data, QIODevice::ReadOnly);

    QList<QNetworkCookie> cookies;
    io >> cookies;
    for (const auto &cookie : std::as_const(cookies)) {
        mCookieJar->deleteCookie(cookie);
        mCookieJar->insertCookie(cookie);
    }
}

int QiliHttp::buid()
{
    auto cookies = mCookieJar->allCookies();
    for (const auto &cookie : std::as_const(cookies)) {
        if (cookie.name() == "DedeUserID") {
            return cookie.value().toInt();
        }
    }
    return 0;
}

QDateTime QiliHttp::expires()
{
    auto cookies = mCookieJar->allCookies();
    auto found = std::find_if(cookies.constBegin(), cookies.constEnd(), [](QNetworkCookie v){
        return v.name() == "SESSDATA";
    });
    if (found != cookies.constEnd()) {
        return found->expirationDate();
    }
    return QDateTime::fromMSecsSinceEpoch(-1);
}

// QSharedPointer<QNetworkReply> QiliHttp::bapi(const QString &url, const QByteArray &data, int timeout)
// {
//     QString md5 = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex().toLower();
//     QString accessKeyId= Qili::ACC_KEY_ID;
//     QString accessKeySecret = Qili::ACC_KEY_SECRET;
//     QString nonce = QUuid::createUuid().toString();
//     qint64 timestamp = QDateTime::currentSecsSinceEpoch();
//     QString sign;
//     sign += "x-bili-accesskeyid:" + accessKeyId + "\n";
//     sign += "x-bili-content-md5:" + md5 + "\n";
//     sign += "x-bili-signature-method:HMAC-SHA256\n";
//     sign += "x-bili-signature-nonce:" + nonce;
//     sign += "\nx-bili-signature-version:1.0\n";
//     sign += QString("x-bili-timestamp:%1").arg(timestamp);
//     QString signature = QMessageAuthenticationCode::hash(
//                             sign.toUtf8(), accessKeySecret.toUtf8(), QCryptographicHash::Sha256).toHex().toLower();

//     QNetworkRequest request = newRequest(url, {}, timeout);

//     request.setRawHeader("x-bili-accesskeyid", accessKeyId.toLocal8Bit());
//     request.setRawHeader("x-bili-content-md5", md5.toLocal8Bit());
//     request.setRawHeader("x-bili-signature-method", "HMAC-SHA256");
//     request.setRawHeader("x-bili-signature-nonce", nonce.toLocal8Bit());
//     request.setRawHeader("x-bili-signature-version", "1.0");
//     request.setRawHeader("x-bili-timestamp", QString("%1").arg(timestamp).toLocal8Bit());
//     request.setRawHeader("Authorization", signature.toLocal8Bit());
//     request.setRawHeader("Accept", "application/json");
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//     QNetworkReply *reply = networkAccessManager->post(request, data);
//     QEventLoop replyWaiter;
//     QObject::connect(reply, &QNetworkReply::finished, &replyWaiter, &QEventLoop::quit);
//     QObject::connect(reply, &QNetworkReply::errorOccurred, &replyWaiter, &QEventLoop::quit);
//     replyWaiter.exec(QEventLoop::ExcludeUserInputEvents);

//     return HttpReply(reply, &QNetworkReply::deleteLater);
// }

QSharedPointer<QNetworkReply> QiliHttp::get(const QString &url, Parameters parameters, int timeout)
{
    QNetworkReply *reply = getAsync(url, parameters, timeout);
    QEventLoop replyWaiter;
    QObject::connect(reply, &QNetworkReply::finished, &replyWaiter, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::errorOccurred, &replyWaiter, &QEventLoop::quit);
    replyWaiter.exec(QEventLoop::ExcludeUserInputEvents);

    return HttpReply(reply, &QNetworkReply::deleteLater);
}

QNetworkReply *QiliHttp::getAsync(const QString &url, Parameters parameters, int timeout)
{
    QNetworkRequest request = newRequest(url, parameters, timeout);
    return networkAccessManager->get(request);
}
