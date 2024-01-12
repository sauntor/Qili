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

#include "QiliSettingsDialog.h"
#include "ui_QiliSettingsDialog.h"

#include "QiliSettings.h"

#include <QSizePolicy>
#include <QMessageBox>


QiliSettingsDialog::QiliSettingsDialog(QiliSettings *settings, QiliSpeaker *speaker, QWidget *parent)
    : QiliDialog(parent)
    , ui(new Ui::QiliSettingsDialog)
    , mSettings(settings)
    , mSpeaker(new QiliSpeaker())
{
    ui->setupUi(this);
    // basic tab
    QObject::connect(ui->userRadio, &QRadioButton::clicked, this, &QiliSettingsDialog::onUserRadioClicked);
    QObject::connect(ui->roomRadio, &QRadioButton::clicked, this, &QiliSettingsDialog::onRoomRadioClicked);

    // voice tab
    QObject::connect(ui->languageCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &QiliSettingsDialog::onLanguageChanged);
    QObject::connect(ui->volumeSpin, qOverload<int>(&QSpinBox::valueChanged), this, &QiliSettingsDialog::onVolumeSpinChanged);
    QObject::connect(ui->pitchSpin, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &QiliSettingsDialog::onPitchSpinChanged);
    QObject::connect(ui->voicesCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &QiliSettingsDialog::onVoiceChanged);
    QObject::connect(ui->volumeSlider, qOverload<int>(&QSlider::valueChanged), this, &QiliSettingsDialog::onVolumeSliderChanged);
    QObject::connect(ui->volumeSpin, qOverload<int>(&QSpinBox::valueChanged), this, &QiliSettingsDialog::onVolumeSpinChanged);
    QObject::connect(ui->pitchSlider, qOverload<int>(&QSlider::valueChanged), this, &QiliSettingsDialog::onPitchSliderChanged);
    QObject::connect(ui->pitchSpin, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &QiliSettingsDialog::onPitchSpinChanged);

    QObject::connect(ui->testButton, &QRadioButton::clicked, this, &QiliSettingsDialog::onTestButtonClicked);
    QObject::connect(ui->applyButton, &QPushButton::clicked, this, &QiliSettingsDialog::onApplyButtonClicked);
}

QiliSettingsDialog::~QiliSettingsDialog()
{
    delete ui;
    delete mSpeaker;
}

void QiliSettingsDialog::show()
{
    mLanguages = mSpeaker->avaiableLanguages();
    mSelectedLang = mSettings->speakerLocale();
    mSelectedVoice = mSettings->speakerVoice();
    auto volume = mSettings->speakerVolume();
    auto pitch = mSettings->speakerPitch();

    ui->volumeSpin->setValue(volume);
    ui->pitchSpin->setValue(pitch);
    ui->roomRadio->setChecked(mSettings->keepRoom());
    ui->userRadio->setChecked(mSettings->keepUser());

    mReset = true;
    setupLanguages();
    mReset = false;

    QiliDialog::show();
}

void QiliSettingsDialog::setupLanguages()
{
    ui->languageCombo->clear();
    // fill languages to the combox
    auto index = 0, selected = index;
    for (const auto &lang : mLanguages) {
        if (mReset && lang == mSelectedLang) {
            selected = index;
        }
        ui->languageCombo->addItem(lang.nativeLanguageName(), lang);
        index++;
    }
    ui->languageCombo->setCurrentIndex(selected);
}

void QiliSettingsDialog::setupVoices()
{
    ui->voicesCombo->clear();

    auto index = 0, selected = index;
    for (const auto &voice : std::as_const(mVoices)) {
        if (mReset && voice.name() == mSelectedVoice) {
            selected = index;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        ui->voicesCombo->addItem(voice.name(), QVariant(std::in_place_type_t<QVoice>(), voice));
#else
        QVariant data;
        data.setValue(voice);
        ui->voicesCombo->addItem(voice.name(), data);
#endif
        index++;
    }

    ui->voicesCombo->setCurrentIndex(selected);
}

void QiliSettingsDialog::onLanguageChanged(int index)
{
    ui->voicesCombo->clear();

    if (index == -1) {
        return;
    }
    const auto lang = ui->languageCombo->itemData(index).value<QLocale>();
    mSpeaker->setLocale(lang);
    mVoices = mSpeaker->avaiableVoices(lang);
    setupVoices();
}

void QiliSettingsDialog::onVoiceChanged(int index)
{
    if (index == -1) {
        return;
    }
    const auto voice = ui->voicesCombo->itemData(index).value<QVoice>();
    mSpeaker->setVoice(voice);
}

void QiliSettingsDialog::onVolumeSpinChanged(int value)
{
    mSpeaker->setVolume(value);
    if (value != ui->volumeSlider->value()) {
        ui->volumeSlider->setValue(value);
    }
}

void QiliSettingsDialog::onVolumeSliderChanged(int value)
{
    if (value != ui->volumeSpin->value()) {
        ui->volumeSpin->setValue(value);
    }
}

void QiliSettingsDialog::onPitchSpinChanged(double value)
{
    mSpeaker->setPitch(value);
    auto v1 = (int)(value * 10);
    if (v1 != ui->pitchSlider->value()) {
        ui->pitchSlider->setValue(v1);
    }
}

void QiliSettingsDialog::onPitchSliderChanged(int value)
{
    auto v1 = value / 10.0;
    if (v1 != ui->pitchSpin->value()) {
        ui->pitchSpin->setValue(v1);
    }
}

void QiliSettingsDialog::onUserRadioClicked(bool checked)
{
    if (checked) {
        mSettings->setKeepUser(true);
        auto button = QMessageBox::question(this, tr("Qili"), tr("Do you want login bilibili.com now?"));
        if (button == QMessageBox::Yes) {
            emit restart();
        }
    }
    else {
        auto button = QMessageBox::question(this, tr("Qili"), tr("Are you sure to clear the authentication for bilibili.com?"));
        if (button == QMessageBox::Yes) {
            mSettings->removeCookies();
            mSettings->setKeepUser(false);
            emit restart();
        }
        else {
            ui->userRadio->setChecked(true);
        }
    }
}

void QiliSettingsDialog::onRoomRadioClicked(bool checked)
{
    if (!checked) {
        auto button = QMessageBox::question(this, tr("Qili"), tr("Are you sure to clear the stored room number?"));
        if (button == QMessageBox::Yes) {
            mSettings->setRoom(0);
        } else {
            ui->roomRadio->setChecked(true);
        }
    }
}

void QiliSettingsDialog::onTestButtonClicked()
{
    auto text = ui->testText->text().trimmed();
    if (!text.isEmpty()) {
        mSpeaker->speak(text);
    }
}

void QiliSettingsDialog::onApplyButtonClicked()
{
    auto lang = ui->languageCombo->currentData().value<QLocale>();
    auto voice = ui->voicesCombo->currentData().value<QVoice>();
    auto volume = ui->volumeSpin->value();
    auto pitch = ui->pitchSpin->value();

    emit apply(lang, voice, volume, pitch);
}
