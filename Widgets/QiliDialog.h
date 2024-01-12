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

#ifndef QILIDIALOG_H
#define QILIDIALOG_H

#include "Config.h"
#include "WidgetsGlobal.h"

#include <QDialog>
#include <QPoint>
#include <QPointF>

class QILI_WIDGETS_EXPORT QiliDialog : public QDialog
{
public:
    QiliDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~QiliDialog();

public slots:
    void show();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPointF           mStart{0, 0};
    QPoint            mPos{0, 0};
#if !QILI_ON_WIN32
    QCursor           mSavedCursor;
#endif
};

#endif // QILIDIALOG_H
