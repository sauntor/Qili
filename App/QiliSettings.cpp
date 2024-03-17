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

#include "QiliSettings.h"

#include "QiliGlobal.h"

#include <QLocale>

using namespace Qili;

QiliSettings::QiliSettings(QObject *parent) :
    QObject(parent),
    mSettings(new QSettings)
{}

QiliSettings::~QiliSettings()
{
    delete mSettings;
}

QiliSettings::ColorMode QiliSettings::colorMode() const
{
    return mSettings->value(SettingKeys::ColorMode, QiliSettings::Auto).value<QiliSettings::ColorMode>();
}

bool QiliSettings::keepUser()
{
    return mSettings->value(SettingKeys::KeepUser, true).toBool();
}

bool QiliSettings::keepRoom()
{
    auto value = mSettings->value(SettingKeys::KeepRoom, true).toBool();
    return value;
}

int QiliSettings::room()
{
    return mSettings->value(SettingKeys::Room, 0).toInt();
}

QByteArray QiliSettings::cookies()
{
    return mSettings->value(SettingKeys::Cookies, QByteArray()).toByteArray();
}

QLocale QiliSettings::speakerLocale() const
{
    return mSettings->value(SettingKeys::Lang, toStorable(QLocale::system())).value<QLocale>();
}

QString QiliSettings::speakerVoice() const
{
    return mSettings->value(SettingKeys::Voice, "").toString();
}

int QiliSettings::speakerVolume() const
{
    return mSettings->value(SettingKeys::Volume, 0).toInt();
}

double QiliSettings::speakerPitch() const
{
    return mSettings->value(SettingKeys::Pitch, 0.0).toDouble();
}

bool QiliSettings::reverseLogs() const
{
    return mSettings->value(SettingKeys::ReverseLogs, true).toBool();
}

QLocale QiliSettings::toStorable(const QLocale &locale)
{
    return QLocale(locale.language(), locale.script(), QLocale::AnyCountry);
}

void QiliSettings::setColorMode(ColorMode value)
{
    mSettings->setValue(SettingKeys::ColorMode, value);
}

void QiliSettings::setKeepUser(bool value)
{
    mSettings->setValue(SettingKeys::KeepUser, value);
}

void QiliSettings::setKeepRoom(bool value)
{
    mSettings->setValue(SettingKeys::KeepRoom, value);
}

void QiliSettings::setReverseLogs(bool value) const
{
    mSettings->setValue(SettingKeys::ReverseLogs, value);
}

void QiliSettings::setRoom(int value)
{
    mSettings->setValue(SettingKeys::Room, value);
}

void QiliSettings::setCookies(const QByteArray &value)
{
    mSettings->setValue(SettingKeys::Cookies, value);
}

void QiliSettings::removeCookies()
{
    mSettings->remove(SettingKeys::Cookies);
}

void QiliSettings::setSpeakerLocale(const QLocale &value)
{
    mSettings->setValue(SettingKeys::Lang, toStorable(value));
}

void QiliSettings::setSpeakerVoice(const QString &value)
{
    mSettings->setValue(SettingKeys::Voice, value);
}

void QiliSettings::setSpeakerVolume(int value)
{
    mSettings->setValue(SettingKeys::Volume, value);
}

void QiliSettings::setSpeakerPitch(double value)
{
    mSettings->setValue(SettingKeys::Pitch, value);
}
