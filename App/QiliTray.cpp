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

#include "QiliTray.h"

#include "QiliLogin.h"
#include "QiliGlobal.h"
#include "Utility.h"

#include <QDebug>
#include <QMap>
#include <QCoreApplication>
#include <QOperatingSystemVersion>
#include <QMessageBox>
#include <QMouseEvent>

using namespace Qili;
using namespace Utility::JSON;
using Utility::Speakable;

QiliTray::QiliTray(QObject *parent)
    : QSystemTrayIcon(parent)
{
    mHttp = new QiliHttp();
    mSettings = new QiliSettings();
    mLauncher = new QiliLauncher(mHttp);
    mLauncher->installEventFilter(this);
    mSubtitleLogger = new QiliSubtitleLogger();

    mSpeaker = new QiliSpeaker();
    mSpeaker->restore(*mSettings);

    mSettingsDialog = new QiliSettingsDialog(mSettings, mSpeaker);
    mThanksDialog = new QiliThanksDialog();

    mConnectAction = new QAction(tr("Re&Connect"));
    mLoggerAction = new QAction(tr("&Logger"));
    mSettingsAction = new QAction(tr("&Settings"));
    mThanksAction = new QAction(tr("&Thanks"));
    mRestartAction = new QAction(tr("&Restart"));
    mExitAction = new QAction(tr("&Exit"));

    menu = new QMenu();
    menu->addAction(mConnectAction);
    menu->addAction(mLoggerAction);
    menu->addAction(mSettingsAction);
    menu->addSeparator();
    menu->addAction(mThanksAction);
    menu->addSeparator();
    menu->addAction(mRestartAction);
    menu->addAction(mExitAction);

    this->setContextMenu(menu);

    this->setToolTip(tr("Qili"));
    this->setIcon(QIcon(":/images/qili.png"));

    int room = mSettings->room();
    mLauncher->setRoom(room);

    bool remember = mSettings->keepUser();
    mLauncher->setRemember(remember);
    if (remember) {
        mHttp->restoreCookies(mSettings->cookies());
    }

    QObject::connect(this, &QiliTray::activated, this, &QiliTray::onIconActived);
    QObject::connect(mLoggerAction, &QAction::triggered, this, &QiliTray::onLoggerTriggered);
    QObject::connect(mConnectAction, &QAction::triggered, this, &QiliTray::onConnectTriggered);
    QObject::connect(mSettingsAction, &QAction::triggered, mSettingsDialog, &QiliSettingsDialog::show);
    QObject::connect(mThanksAction, &QAction::triggered, this, &QiliTray::onThanksTriggered);
    QObject::connect(mRestartAction, &QAction::triggered, this, &QiliTray::onRestart);
    QObject::connect(mExitAction, &QAction::triggered, qApp, &QCoreApplication::exit);

    QObject::connect(mLauncher, &QiliLauncher::rememberChanged, this, &QiliTray::onRememberChanged);
    QObject::connect(mLauncher, &QiliLauncher::starting, this, qOverload<UserRole, int>(&QiliTray::connect));
    auto expires = mHttp->expires();
    if (expires.isValid() && expires.toMSecsSinceEpoch() > QDateTime::currentMSecsSinceEpoch()) {
        mLauncher->setAuthenticated(true);
    }

    QObject::connect(mSettingsDialog, &QiliSettingsDialog::apply, this, &QiliTray::onVoiceApply);
    QObject::connect(mSettingsDialog, &QiliSettingsDialog::restart, this, &QiliTray::onRestart);
}

QiliTray::~QiliTray()
{
    delete mHttp;
    delete mSettings;
    delete mSettingsDialog;
    delete mLauncher;
    delete mSubtitleLogger;
    delete mSpeaker;
    delete menu;
}

void QiliTray::show()
{
    if (started) {
        QSystemTrayIcon::show();
        return;
    }
    mLauncher->show();
}

void QiliTray::connect(UserRole role, int room)
{
    // always store room to settings
    mSettings->setRoom(room);
    mRoom = room;
    mRole = role;

    if (role == UserRole::Anonymous) {
        mSettings->removeCookies();
        // clear cookies if any
        mHttp->restoreCookies();
    }
    else {
        mSettings->setCookies(mHttp->storabelCookies());
    }


    if (mConnection != nullptr) {
        mConnection->disconnect();
        mConnection->deleteLater();
    }
    mConnection = new QiliConnection(mHttp);
    mConnection->setRoom(room);
    mConnection->connect();
    QObject::connect(mConnection, &QiliConnection::authenticated, this, &QiliTray::onAuthenticated);
    QObject::connect(mConnection, &QiliConnection::subtitleReceived, this, &QiliTray::onSubtitleReceived);
    // QObject::connect(mConnection, &QiliConnection::watchersChanged, this, &QiliTray::onWatchersChanged);
    QObject::connect(mConnection, &QiliConnection::errorOccured, this, &QiliTray::onConnectionError);

}

QiliSettings &QiliTray::settings() const
{
    return *mSettings;
}

bool QiliTray::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Close && watched == mLauncher) {
        if (!this->started) {
            qApp->exit();
        }
    }
    return QSystemTrayIcon::eventFilter(watched, event);
}

void QiliTray::onLoggerTriggered()
{
    mSubtitleLogger->show();
    mSubtitleLogger->activateWindow();
    mSubtitleLogger->raise();
}

void QiliTray::onRememberChanged(bool remember)
{
    if (!remember) {
        mSettings->removeCookies();
    }
    mSettings->setKeepUser(remember);
}

void QiliTray::onConnectTriggered()
{
    connect(mRole, mRoom);
}

void QiliTray::onThanksTriggered()
{
    mThanksDialog->show();
    mThanksDialog->activateWindow();
}

void QiliTray::onAuthenticated()
{
    started = true;
    show();
    showMessage(tr("Qili"), tr("Connected now, to exit use the system tray."));
    mLauncher->close();
    // onLoggerTriggered();
    emit connected();
}

void QiliTray::onSubtitleReceived(const QJsonObject &subtitle)
{
    QString cmd = subtitle / "cmd" >> JString;

    QString text;

    auto data = subtitle / "data" >> JObject;
    if (cmd == "DANMU_MSG") {
        QJsonArray data = subtitle / "info" >> JArray;
        QString uname = data[2].toArray()[1] >> JString;
        QString msg = data[1] >> JString;
        auto extra = data[0].toArray()[15] >> JString >> JObject;
        text = tr("%1 says %2").arg(Speakable::username(uname)).arg(msg);
        QString now = QDateTime::currentDateTime().toString("MM/dd hh:mm:ss");
        mSubtitleLogger->subtitleReceived(QString("[%1] %2").arg(now).arg(text));
    }
    else if (cmd == "INTERACT_WORD") {
        QString uname = subtitle / "data" / "uinfo" / "base" / "name" >> JString;
        text = tr("%1 enter room").arg(Speakable::username(uname));
    }
    else if (cmd == "ENTRY_EFFECT") {
        text = subtitle / "data" / "copy_writing" >> JString;
    }
    else if (cmd == "ONLINE_RANK_V2") {
        auto data = subtitle / "data" >> JObject;
        QJsonArray list;
        if (data.contains("list")) {
            list = data / "list" >> JArray;
        }
        else if (data.contains("online_list")) {
            list = data / "online_list" >> JArray;
        }
        subtitle / "data" / "list" >> JArray;
        QMap<int, QString> ranks;
        for (const auto &item : std::as_const(list)) {
            auto rank = item.toObject();
            QString uname = rank / "uname" >> JString;
            int value = rank / "rank" >> JInt;
            ranks[value] = uname;
        }
        qDebug() << "ranks = " << ranks;
    }
    else if (cmd == "PK_BATTLE_END") {
        // auto begin = data / "init_info" / "best_uname" >> JString;
        // auto end = data / "match_info" / "best_uname" >> JString;
        // text = QString("most helpful warrior, %1 at begin, %2 at end").arg(begin).arg(end);
    }
    else if (cmd == "SEND_GIFT") {
        auto action = data / "action" >> JString;
        auto uname = data / "uname" >> JString;
        auto gift = data / "giftName" >> JString;
        auto batch = data / "super_gift_num" >> JInt;
        auto num = data / "num" >> JInt;
        text = tr("%1 %2 %3 %4 x %5").arg(Speakable::username(uname)).arg(action).arg(num).arg(gift).arg(batch);
    }
    else if (cmd == "LIKE_INFO_V3_CLICK") {
        auto uname = data / "uname" >> JString;
        auto like = data / "like_text" >> JString;
        text = uname + like;
    }
    else if (cmd == "NOTICE_MSG") {
        auto realRoom = subtitle / "real_roomid" >> JInt;
        auto message = subtitle / "msg_self" >> JString;
        if (realRoom == mRoom) {
            text = message;
        }
    }
    else if (cmd == "ONLINE_RANK_COUNT") {
        //在线人数人数变更
        auto count = data / "count" >> JInt;
        auto onlineCount = data / "online_count" >> JInt;
    }
    else if (cmd == "WATCHED_CHANGE") {
        //几人看过本场直播
        auto count = data / "num" >> JInt;
        //描述文本
        // text = data / "text_large" >> JString;
        text = tr("%1 visitors until now").arg(count);
    }

    if (!text.isEmpty()) {
        mSpeaker->speak(text);
        mSubtitleLogger->subtitleReceived(text);
    }
}

void QiliTray::onWatchersChanged(int watchers)
{
    mWatchers = watchers;
    setToolTip(QString(tr("%1 Watcher(s)")).arg(mWatchers));
    mSubtitleLogger->subtitleReceived(QString(tr("[%1] Watcher(s): %2")).arg(QDateTime::currentDateTime().toString("MM/dd hh:mm:ss")).arg(watchers));
}

void QiliTray::onIconActived(ActivationReason reason)
{
    qDebug() << "Activated = " << reason;
    switch(reason) {
    case ActivationReason::Context:
        // nothing need to do
        break;
    case ActivationReason::DoubleClick:
    case ActivationReason::MiddleClick:
        qDebug() << "Activated = MiddleClick";
        if (paused) {
            mSpeaker->resume();
            paused = false;
            showMessage(tr("Qili Speaker"), tr("Resumed"));
        } else {
            mSpeaker->pause();
            paused = true;
            showMessage(tr("Qili Speaker"), tr("Paused"));
        }
        break;
    default:
        qDebug() << "Activated = Trigger";
        onLoggerTriggered();
        break;
    }
}

void QiliTray::onConnectionError(QiliConnection::Error error, const QString &errorString)
{
    showMessage(tr("Qili disconnected"), errorString);
    if (mConnection != nullptr) {
        mConnection->disconnect();
        mConnection->deleteLater();
    }
}

void QiliTray::onVoiceApply(const QLocale &locale, const QVoice &voice, int volume, double pitch)
{
    mSpeaker->setLocale(locale);
    mSpeaker->setVoice(voice);
    mSpeaker->setVolume(volume);
    mSpeaker->setPitch(pitch);

    mSettings->setSpeakerLocale(locale);
    mSettings->setSpeakerVoice(voice.name());
    mSettings->setSpeakerVolume(volume);
    mSettings->setSpeakerPitch(pitch);
    qInfo() << "Speaker changed: locale = " << locale << " voice = " << voice.name();
    QMessageBox::information(nullptr, tr("Qili Speaker"), tr("Settings applied"));
}

void QiliTray::onRestart()
{
    qApp->exit(QiliFlag::Restart);
}
