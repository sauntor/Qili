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

#ifndef QILIQRCODE_H
#define QILIQRCODE_H

#include "ThirdpartyExports.h"

#include <QObject>

struct QRCodeData;

class QILI_THIRDPARTY_EXPORT QiliQRCode : public QObject
{
    Q_OBJECT

public:
    explicit QiliQRCode(const QString &content, QObject *parent = nullptr);
    ~QiliQRCode();

    int size() const;
    bool module(int x, int y) const;

private:
    QRCodeData *mData;

};

#endif // QILIQRCODE_H
