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

#include "QiliSpeaker.h"

#include "QiliSettings.h"

#include <QDebug>
#include <QSet>
#include <QList>


const QList<QLocale> QiliSpeaker::SupportedLanguages = {
    QLocale(QLocale::English, QLocale::UnitedStates),
    QLocale(QLocale::Chinese, QLocale::SimplifiedChineseScript, QLocale::AnyCountry)
};


QiliSpeaker::QiliSpeaker(QObject *parent)
    : QObject(parent)
{
    mTextToSpeech = new QTextToSpeech(this);
}

QiliSpeaker::~QiliSpeaker()
{}

bool QiliSpeaker::isLanguageSupported(const QLocale &locale) {
    for (const auto &l : std::as_const(SupportedLanguages)) {
        if (l.language() == locale.language() && (l.script() == QLocale::AnyScript || l.script() == locale.script())) {
            return true;
        }
    }
    return false;
}

void QiliSpeaker::restore(const QiliSettings &settings)
{
    auto lang = settings.speakerLocale();
    auto voice = settings.speakerVoice();
    auto volume = settings.speakerVolume();
    auto pitch = settings.speakerPitch();
    mTextToSpeech->setLocale(lang);
    mTextToSpeech->setVolume(volume);
    mTextToSpeech->setPitch(pitch);
    if (!voice.isEmpty()) {
        auto voices = avaiableVoices(lang);
        auto found = std::find_if(voices.cbegin(), voices.cend(), [&](const QVoice &it) -> bool {
            return it.name() == voice;
        });
        if (found != voices.cend()) {
            mTextToSpeech->setVoice(*found);
        }
    }
}


void QiliSpeaker::setLocale(const QLocale &locale)
{
    mTextToSpeech->setLocale(locale);
}

QList<QLocale> QiliSpeaker::avaiableLanguages()
{
    auto clocale = QLocale::c();
    auto system = QLocale::system();
    auto locales = mTextToSpeech->availableLocales();
    QSet<QLocale> distincted;
    distincted += QiliSettings::toStorable(system);
    distincted += QiliSettings::toStorable(QLocale(QLocale::Chinese, QLocale::SimplifiedChineseScript, QLocale::AnyCountry));
    for (const auto &locale : std::as_const(locales)) {
        if (locale == clocale || !isLanguageSupported(locale)) {
            continue;
        }
        distincted += QiliSettings::toStorable(locale);
    }
    return QList<QLocale>(distincted.begin(), distincted.end());
}

QList<QVoice> QiliSpeaker::avaiableVoices(const QLocale &language)
{
    const auto langName = QLocale::languageToString(language.language());
    const auto cLocale = QLocale::c();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QList<QVoice> voices = mTextToSpeech->findVoices(QLocale::C);

    auto currentLocale = mTextToSpeech->locale();

    for (const auto &supportedLanguage : std::as_const(SupportedLanguages)) {
        mTextToSpeech->setLocale(supportedLanguage);
        auto localized = mTextToSpeech->findVoices(supportedLanguage.language());
        voices += localized;
    }
    mTextToSpeech->setLocale(currentLocale);

    QList<QVoice> avaiables;
    for (const auto &voice : std::as_const(voices)) {
        auto locale = QiliSettings::toStorable(voice.locale());
        bool supported = false;
        for (const auto &supportedLocale : std::as_const(SupportedLanguages)) {
            const auto &name = voice.name();
            if ((locale == cLocale && name.startsWith(langName))
                || (supportedLocale.language() == locale.language()
                    && (supportedLocale.script() == QLocale::AnyScript
                        || supportedLocale.script() == locale.script()
                        || name.startsWith(langName))
                    && (language.script() == QLocale::AnyScript
                        || language.script() == locale.script()
                        || name.startsWith(langName)))
                ) {
                supported = true;
                break;
            }
        }
        if (supported) {
            avaiables += voice;
        }
    }
    return avaiables;
#else
    auto originalLocale = mTextToSpeech->locale();
    mTextToSpeech->setLocale(language);
    QVector<QVoice> voices = mTextToSpeech->availableVoices();
    mTextToSpeech->setLocale(QLocale::c());
    voices += mTextToSpeech->availableVoices();
    QList<QVoice> avaiables;
    for (const auto &voice : std::as_const(voices)) {
        if(voice.name().contains(langName)) {
            avaiables += voice;
        }
    }
    mTextToSpeech->setLocale(originalLocale);
    return avaiables;
#endif
}

QLocale QiliSpeaker::locale() const
{
    return mTextToSpeech->locale();
}

QVoice QiliSpeaker::voice() const
{
    return mTextToSpeech->voice();
}

void QiliSpeaker::setVoice(const QVoice &voice)
{
    mTextToSpeech->setVoice(voice);
}

void QiliSpeaker::setVolume(int volume)
{
    mTextToSpeech->setVolume(volume / 100.0);
}

int QiliSpeaker::volume() const
{
    auto volume = mTextToSpeech->volume();
    return (int) (volume * 100);
}

double QiliSpeaker::pitch() const
{
    return mTextToSpeech->pitch() + 1;
}

void QiliSpeaker::setPitch(double pitch)
{
    mTextToSpeech->setPitch(pitch - 1);
}

void QiliSpeaker::speak(const QString &text)
{
    qDebug() << "Speaking: " << qUtf8Printable(text);
    mTextToSpeech->say(text);
}

bool QiliSpeaker::supportPause() const
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return mTextToSpeech->engineCapabilities() & QTextToSpeech::Capability::PauseResume;
#else
    // Qt5 does not provide an api to detect it, let's assume it as supported
    return true;
#endif
}

void QiliSpeaker::pause()
{
    mTextToSpeech->pause();
}

void QiliSpeaker::resume()
{
    mTextToSpeech->resume();
}
