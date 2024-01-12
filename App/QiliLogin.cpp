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


#include "QiliLogin.h"
#include "ui_QiliLogin.h"

#include "QiliGlobal.h"
#include "Utility.h"

#include "QiliQRCode.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

using namespace Qili;
using namespace Utility::JSON;

QiliLauncher::QiliLauncher(QiliHttp *http, QWidget *parent) :
    QiliDialog(parent),
    ui(new Ui::QiliLauncher), mHttp(http)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    ui->rememberButton->setChecked(mRemember);
    connect(ui->startButton, &QPushButton::clicked, this, &QiliLauncher::onStartClicked);
    connect(ui->rememberButton, &QRadioButton::toggled, this, &QiliLauncher::onRememberToggled);
    connect(ui->qrcode, &QPushButton::clicked, this, &QiliLauncher::refreshQrCode);
    connect(ui->anonymous, &QRadioButton::toggled, this, &QiliLauncher::onAnonymousToggled);
}

QiliLauncher::~QiliLauncher()
{
    delete ui;
}


// overrides
void QiliLauncher::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != mQrTimer) {
        QDialog::timerEvent(event);
        return;
    }

    if (mQrTTL > 0) {
        pullQrcode();
    }
    else {
        clearPulling();
        QMessageBox::warning(this, tr("Scan QrCode"), tr("QrCode goes invalid"));
    }
    mQrTTL--;
}

void QiliLauncher::show()
{
    emit ui->anonymous->toggled(mAuthenticated);
    if (!mAuthenticated) {
        refreshQrCode();
    }
    QiliDialog::show();
}

// properites
void QiliLauncher::setRemember(bool remember)
{
    ui->rememberButton->setCheckable(remember);
}

void QiliLauncher::setRoom(int room)
{
    if (room == 0) {
        ui->roomField->clear();
    }
    else {
        ui->roomField->setText(QString("%1").arg(room));
    }
}

void QiliLauncher::setAuthenticated(bool authenticated)
{
    mAuthenticated = authenticated;
    emit ui->anonymous->toggled(authenticated);
}


// ui interactions
void QiliLauncher::showError(const QString &message)
{
    QMessageBox::warning(this, tr("Scan QrCode"), message);
}

void QiliLauncher::onStartClicked()
{
    auto room = ui->roomField->text().toInt();
    auto role = UserRole::Anonymous;
    if (!ui->anonymous->isChecked()) {
        role = UserRole::Authenticated;
    }
    emit starting(role, room);
}

void QiliLauncher::onRememberToggled(bool checked)
{
    mRemember = checked;
}

void QiliLauncher::onAnonymousToggled(bool checked)
{
    ui->rememberWrapper->setVisible(mAuthenticated || !checked);
    ui->qrcode->setVisible(!checked);
    ui->roomField->setVisible(checked);
    ui->startWidget->setVisible(checked);
    emit ui->rememberButton->toggled(!checked);
    if (checked) {
        clearPulling();
    }
    else {
        refreshQrCode();
    }
}


// methods
void QiliLauncher::refreshQrCode()
{
    clearPulling();
    auto reply = mHttp->get(Qili::Uris::GenerateQrcode);
    if (reply->error() != QNetworkReply::NoError) {
        showError(reply->errorString());
        return;
    }
    auto json = reply->readAll() >> JObject;
    int code = json >> JInt;
    if (code != 0)  {
        showError(json >> JString);
        return;
    }

    QString url = json / "data" / "url" >> JString;
    mQrKey = json / "data" / "qrcode_key" >> JString;

    QiliQRCode qr(url);
    int size = qr.size();
    QImage image(size, size, QImage::Format_RGB32);
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (qr.module(y, x)) {
                image.setPixel(x, y, qRgb(255, 255, 255));
            } else {
                image.setPixel(x, y, qRgb(0, 0, 0));
            }
        }
    }

    QImage scaled = image.scaled(ui->qrcode->width(), ui->qrcode->height(), Qt::KeepAspectRatio);
    ui->qrcode->setIcon(QPixmap::fromImage(scaled));

    qDebug() << "QRCode Generated: " << json;
    if (mQrTimer != 0) {
        killTimer(mQrTimer);
    }
    mQrTimer = startTimer(10 * 1000);
    mQrTTL = 10 * 60 / 10;
    emit refreshed();
}

void QiliLauncher::pullQrcode()
{
    qDebug() << "QRCode Pulling...";
    auto reply = mHttp->get(Qili::Uris::PollQrcode, {{"qrcode_key", mQrKey}});
    if (reply->error()) {
        showError(reply->errorString());
        return;
    }
    auto json = reply->readAll() >> JObject;
    qDebug() << "QRCode Pulled Success: " << json;

    // qDebug() << "QRCode Login Cookie: " ;
    // auto cookieJar = mHttp->cookieJar();
    // auto cookies = cookieJar->allCookies();
    // for (const auto &cookie : std::as_const(cookies)) {
    //     qDebug() << cookie;
    // }

    int code = json / "data" / "code" >> JInt;
    if (code == 86038) {
        clearPulling();
        showError(json / "data" / "message" >> JString);
    }
    else if (code == 0) {
        qDebug() << "QRCode Login Success";
        clearPulling();

        mAuthenticated = true;
        ui->qrcode->setVisible(false);
        ui->roomField->setVisible(true);
        ui->anonymousWrapper->setVisible(false);
        ui->startWidget->setVisible(true);
    }
}


void QiliLauncher::clearPulling()
{
    qDebug() << "QRCode Clear Pulling Timer: " << mQrTimer;
    if (mQrTimer != 0) {
        killTimer(mQrTimer);
        mQrTimer = 0;
    }
}
