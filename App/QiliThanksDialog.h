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

#ifndef QILITHANKSDIALOG_H
#define QILITHANKSDIALOG_H

#include "QiliDialog.h"

#include <QDialog>

namespace Ui {
class QiliThanksDialog;
}

class QiliThanksDialog : public QiliDialog
{
    Q_OBJECT

public:
    explicit QiliThanksDialog(QWidget *parent = nullptr);
    ~QiliThanksDialog();

private:
    Ui::QiliThanksDialog *ui;
};

#endif // QILITHANKSDIALOG_H
