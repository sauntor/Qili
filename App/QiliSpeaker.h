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

#ifndef QILISPEAKER_H
#define QILISPEAKER_H

#include "QiliAppGlobal.h"

#include <QObject>
#include <QTextToSpeech>
#include <QLocale>
#include <QVoice>
#include <QSettings>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(QVoice)
#endif

class QiliSettings;

class QILI_APP_EXPORT QiliSpeaker : public QObject
{
    Q_OBJECT
public:
    explicit QiliSpeaker(QObject *parent = nullptr);
    ~QiliSpeaker();

    QList<QLocale> avaiableLanguages();
    QList<QVoice>  avaiableVoices(const QLocale &language);
    QLocale        locale() const;
    QVoice         voice() const;
    int            volume() const;
    double         pitch() const;
    void           restore(const QiliSettings &settings);

public slots:
    void setLocale(const QLocale &locale);
    void setVoice(const QVoice &voice);
    void setVolume(int volume);
    void setPitch(double pitch);
    void speak(const QString &text);
    bool supportPause() const;
    void pause();
    void resume();

private:
    static const QList<QLocale> SupportedLanguages;
    bool isLanguageSupported(const QLocale &locale);

    QTextToSpeech *mTextToSpeech;
};

#endif // QILISPEAKER_H
