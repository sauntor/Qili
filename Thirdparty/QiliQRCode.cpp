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

#include "QiliQRCode.h"

#include <qrcodegen.hpp>

using namespace qrcodegen;

struct QRCodeData
{
    QrCode code;
};

QiliQRCode::QiliQRCode(const QString &content, QObject *parent)
    : QObject{parent}
{
    auto qrcode = QrCode::encodeText(qUtf8Printable(content), QrCode::Ecc::LOW);
    mData = new QRCodeData{qrcode};
}

QiliQRCode::~QiliQRCode()
{
    delete mData;
}

int QiliQRCode::size() const
{
    return mData->code.getSize();
}

bool QiliQRCode::module(int x, int y) const
{
    return mData->code.getModule(x, y);
}
