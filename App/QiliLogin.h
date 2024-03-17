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

#ifndef QILILOGIN_H
#define QILILOGIN_H

#include "QiliAppGlobal.h"
#include "QiliHttp.h"
#include "QiliWidget.h"

#include <QDialog>

namespace Ui {
class QiliLauncher;
}

class QDrag;

enum UserRole { Anonymous, Authenticated };

class QILI_APP_EXPORT QiliLauncher : public QiliDialog
{
    Q_OBJECT

public:
    explicit QiliLauncher(QiliHttp *http, QWidget *parent = nullptr);
    ~QiliLauncher();

    void show();

signals:
    void refreshed();
    void rememberChanged(bool remember);
    void starting(UserRole role, int room);

public slots:
    void setRemember(bool remember);
    void setRoom(int room);
    void setAuthenticated(bool authenticated);


protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void onRememberToggled(bool checked);
    void onAnonymousToggled(bool checked);
    void onStartClicked();

    void refreshQrCode();

private:
    void clearPulling();
    void pullQrcode();
    void showError(const QString &message);

    Ui::QiliLauncher *ui;
    QiliHttp         *mHttp;

    bool              mAuthenticated{false};
    bool              mRemember{true};
    int               mQrTTL{0};
    QString           mQrKey;
    int               mQrTimer{0};
    QPointF           mStart{0, 0};
    QPoint            mPos{0, 0};
};

#endif // QILILOGIN_H
