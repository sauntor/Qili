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

#include "QiliTitleBar.h"
#include "ui_QiliTitleBar.h"

QiliTitleBar::QiliTitleBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QiliTitleBar)
{
    ui->setupUi(this);
    ui->titleLabel->setText(QWidget::windowTitle());
    QObject::connect(ui->closeButton, &QPushButton::clicked, this, &QiliTitleBar::onCloseClicked);
}

QiliTitleBar::~QiliTitleBar()
{
    delete ui;
}

QString QiliTitleBar::text() const
{
    return ui->titleLabel->text();
}

void QiliTitleBar::setText(const QString &text)
{
    if (text != ui->titleLabel->text()) {
        ui->titleLabel->setText(text);
        emit textChanged(text);
    }
}

void QiliTitleBar::onCloseClicked()
{
    emit closing();
}
