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

#ifndef QILISETTINGS_H
#define QILISETTINGS_H

#include "QiliAppGlobal.h"

#include <QSettings>

class QILI_APP_EXPORT QiliSettings : public QObject
{
    Q_OBJECT
public:
    explicit QiliSettings(QObject *parent = nullptr);
    ~QiliSettings();

    bool    keepUser();
    bool    keepRoom();
    int     room();
    QByteArray cookies();

    QLocale speakerLocale() const;
    QString speakerVoice() const;
    int     speakerVolume() const;
    double  speakerPitch() const;

    bool    reverseLogs() const;

    static QLocale toStorable(const QLocale &locale);

public slots:
    void setKeepUser(bool value);
    void setKeepRoom(bool value);
    void setReverseLogs(bool value) const;
    void setRoom(int value);
    void setCookies(const QByteArray &value);
    void removeCookies();
    void setSpeakerLocale(const QLocale &value);
    void setSpeakerVoice(const QString &value);
    void setSpeakerVolume(int value);
    void setSpeakerPitch(double value);

private:
    QSettings *mSettings;
};

#endif // QILISETTINGS_H
