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

#ifndef QILISUBTITLELOGGER_H
#define QILISUBTITLELOGGER_H

#include "QiliAppGlobal.h"
#include "QiliDialog.h"

#include <QDialog>

namespace Ui {
class QiliSubtitleLogger;
}

class QILI_APP_EXPORT QiliSubtitleLogger : public QiliDialog
{
    Q_OBJECT

public:
    explicit QiliSubtitleLogger(QWidget *parent = nullptr);
    ~QiliSubtitleLogger();

    int maxLines() const;
    bool reversed() const;

public slots:
    void subtitleReceived(const QString &message);
    void setMaxLines(int maxLines);
    void setReversed(bool reversed);

private:
    int                     mMaxLines{2};
    bool                    mReversed{true};
    Ui::QiliSubtitleLogger *ui;
};

#endif // QILISUBTITLELOGGER_H
