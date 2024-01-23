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

#ifndef QILITRAY_H
#define QILITRAY_H

#include "QiliAppGlobal.h"
#include "QiliHttp.h"
#include "QiliConnection.h"
#include "QiliLogin.h"
#include "QiliSubtitleLogger.h"
#include "QiliSpeaker.h"
#include "QiliSettings.h"
#include "QiliSettingsDialog.h"
#include "QiliThanksDialog.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>


class QILI_APP_EXPORT QiliTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit QiliTray(QObject *parent = nullptr);
    ~QiliTray();

signals:
    void connected();
    void errorOccured(QString message);

public slots:
    void show();
    void connect(UserRole role, int room);

public:
    QiliSettings &settings() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onRememberChanged(bool remember);

    void onIconActived(QSystemTrayIcon::ActivationReason reason);

    void onLoggerTriggered();
    void onConnectTriggered();
    void onThanksTriggered();
    void onShowLogsTriggered();

    void onAuthenticated();
    void onSubtitleReceived(const QJsonObject &subtitle);
    void onWatchersChanged(int watchers);

    void onConnectionError(QiliConnection::Error error, const QString &errorString);
    void onVoiceApply(const QLocale &locale, const QVoice &voice, int volume, double pitch);
    void onRestart();

private:
    int  mRoom;
    UserRole mRole;
    bool started{false};
    bool paused{false};
    int  mWatchers{0};

    QiliSettings *mSettings;

    QMenu     *menu;
    QAction   *mConnectAction;
    QAction   *mLoggerAction;
    QAction   *mSettingsAction;
    QAction   *mThanksAction;
    QAction   *mRestartAction;
    QAction   *mExitAction;
    QAction   *mShowLogsAction;

    QiliLauncher       *mLauncher;
    QiliSettingsDialog *mSettingsDialog;
    QiliSubtitleLogger *mSubtitleLogger;
    QiliThanksDialog   *mThanksDialog;
    QiliConnection     *mConnection{nullptr};
    QiliHttp           *mHttp;
    QiliSpeaker        *mSpeaker;
};

#endif // QILITRAY_H
