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

#include "QiliTextField.h"

#include <QSizePolicy>

QiliTextField::QiliTextField(QWidget *parent)
    : QWidget{parent}
{
    this->setProperty("qili-widget", "text-field");

    mText = new QLineEdit(this);
    mText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    mLabel = new QLabel("Field", this);

    mClear = new QPushButton(this);
    mClear->setFlat(true);
    mClear->setGeometry(0, 0, 16, 16);
    mClear->setProperty("qili-btn", "clear");

    mLayout = new QHBoxLayout();
    mLayout->addWidget(mLabel, 3);
    mLayout->addWidget(mText, 8);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(mLayout);
    this->setMinimumHeight(31);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setGeometry(0, 0, 200, 31);
    this->setContentsMargins(0, 0, 0, 0);

    QObject::connect(mClear, &QPushButton::clicked, this, &QiliTextField::onClearClicked);
    QObject::connect(mText, &QLineEdit::textChanged, this, &QiliTextField::onTextChanged);
}

void QiliTextField::setLabel(const QString &label)
{
    mLabel->setText(label);
}

QString QiliTextField::label() const
{
    return mLabel->text();
}

void QiliTextField::setText(const QString &text)
{
    mText->setText(text);
}

QString QiliTextField::text() const
{
    return mText->text();
}

void QiliTextField::setEchoMode(QLineEdit::EchoMode value)
{
    mText->setEchoMode(value);
}

QLineEdit::EchoMode QiliTextField::echoMode() const
{
    return mText->echoMode();
}

void QiliTextField::setPlaceholder(const QString &text)
{
    mText->setPlaceholderText(text);
}

QString QiliTextField::placeholder() const
{
    return mText->placeholderText();
}

void QiliTextField::setAlignment(Qt::Alignment alignment)
{
    mLabel->setAlignment(alignment);
}

Qt::Alignment QiliTextField::alignment() const
{
    return mLabel->alignment();
}

void QiliTextField::clear()
{
    mText->clear();
}

void QiliTextField::resizeEvent(QResizeEvent *event)
{
    auto width = mText->height() / 11.0;
    auto height = width;
    auto x = this->width() - width * 9;
    auto y = this->height() - height * 9;
    mClear->setGeometry(qRound(x), qRound(y), qRound(width * 7), qRound(height * 7));
    auto margins = mText->textMargins();
    margins.setRight(qRound(width * 9));
    mText->setTextMargins(margins);
    QWidget::resizeEvent(event);
}

void QiliTextField::onClearClicked()
{
    mText->clear();
}

void QiliTextField::onTextChanged(const QString &text)
{
    emit textChanged(text);
}
