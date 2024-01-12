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

#include "QiliDialog.h"

#include <QDebug>
#include <QMouseEvent>

QiliDialog::QiliDialog(QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{}

QiliDialog::~QiliDialog()
{}

void QiliDialog::mousePressEvent(QMouseEvent *event)
{
    QDialog::mousePressEvent(event);
    if (event->button() != Qt::LeftButton) {
        return;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    auto child = childAt(event->scenePosition().toPoint());
#else
    auto child = childAt(event->localPos().toPoint());
#endif
    bool should = child == nullptr;
    if (!should) {
        auto meta = child->metaObject();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        should = std::strcmp(meta->className(), "QWidget") == 0
                 || std::strcmp(meta->className(), "QiliTitleBar") == 0;
#else
        should = strcmp(meta->className(), "QWidget") == 0
                 || strcmp(meta->className(), "QiliTitleBar") == 0;
#endif
    }
    if (!should) {
        auto *parent = child->parentWidget();
        should = parent != nullptr && parent->property("qili-widget").toString() == "titlebar";
    }

    if (should) {
#if QILI_ON_WIN32
        // no need to change cursor
#else
        mSavedCursor = cursor();
        setCursor(Qt::SizeAllCursor);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        mStart = event->globalPosition();
#else
        mStart = event->globalPos();
#endif
        mPos = pos();
        qDebug() << "Move Start: @" << mPos;
    } else {
        qDebug() << "Found Child: " << child->metaObject()->className() << " => " << child->objectName();
    }
}

void QiliDialog::mouseReleaseEvent(QMouseEvent *event)
{
    QDialog::mouseReleaseEvent(event);
    if (event->button() != Qt::LeftButton) {
        return;
    }
    mStart = QPointF(0, 0);
    qDebug() << "Move Finised: " << mPos;
#if QILI_ON_WIN32
    // nothing need to do
#else
    setCursor(mSavedCursor);
#endif
}

void QiliDialog::mouseMoveEvent(QMouseEvent *event)
{
    QDialog::mouseMoveEvent(event);
    qDebug() << "Move Moving: @" << mStart;
    if (!mStart.isNull()) {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        auto current = event->globalPosition();
#else
        auto current = event->globalPos();
#endif
        qDebug() << "Move Moving: start = " << mStart << ", pos = " << mPos << ", current = " << current;
        move(mPos.x() + current.x() - mStart.x(), mPos.y() + current.y() - mStart.y());
    }
}

void QiliDialog::show()
{
    QDialog::show();
    activateWindow();
    raise();
}
