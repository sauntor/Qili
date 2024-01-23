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

#ifndef QILISETTINGSDIALOG_H
#define QILISETTINGSDIALOG_H

#include "QiliAppGlobal.h"
#include "QiliSpeaker.h"
#include "QiliDialog.h"

#include <QDialog>
#include <QSettings>

namespace Ui {
class QiliSettingsDialog;
}

class QiliSettings;

class QILI_APP_EXPORT QiliSettingsDialog : public QiliDialog
{
    Q_OBJECT

public:
    explicit QiliSettingsDialog(QiliSettings *settings, QiliSpeaker *speaker, QWidget *parent = nullptr);
    ~QiliSettingsDialog();
    
    void setupLanguages();
    
    void setupVoices();
    
signals:
    void restart();
    void apply(const QLocale &locale, const QVoice &voice, int volume, double pitch);
    void reverseChanged(bool reverse);

public slots:
    void show();

private slots:
    void onLanguageChanged(int index);
    void onVoiceChanged(int index);
    void onVolumeSpinChanged(int value);
    void onVolumeSliderChanged(int value);
    void onPitchSpinChanged(double value);
    void onPitchSliderChanged(int value);
    void onUserRadioClicked(bool checked);
    void onRoomRadioClicked(bool checked);
    void onReverseRadioClicked(bool checked);
    void onTestButtonClicked();
    void onApplyButtonClicked();

private:
    Ui::QiliSettingsDialog *ui;
    QiliSpeaker            *mSpeaker;
    QiliSettings              *mSettings;
    QList<QLocale>          mLanguages;
    QList<QVoice>           mVoices;
    QString                 mSelectedVoice;
    QLocale                 mSelectedLang;
    bool                    mReset{true};
};

#endif // QILISETTINGSDIALOG_H
