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

#ifndef QILIWIDGET_H
#define QILIWIDGET_H

#include "Config.h"
#include "WidgetsGlobal.h"

#include <QDebug>
#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPoint>
#include <QPointF>

///////// Declarations ////////
template<class T = QWidget/* T must be a subtype of QWidget */>
class QILI_WIDGETS_EXPORT QiliWidget : public T
{
public:
    QiliWidget(QWidget *parent = nullptr);
    ~QiliWidget();

public slots:
    void show();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QPointF           mStart{0, 0};
    QPoint            mPos{0, 0};
#if !QILI_ON_WIN32
    QCursor           mSavedCursor;
#endif
};

class QiliDialog : public QiliWidget<QDialog>
{
public:
    QiliDialog(QWidget *parent = nullptr) : QiliWidget(parent) {};
    ~QiliDialog() {};
};


////////// Implementations ///////////
template<class T>
QiliWidget<T>::QiliWidget(QWidget *parent)
    :T(parent)
{
    T::setWindowFlag(Qt::FramelessWindowHint);
    T::setAttribute(Qt::WA_TranslucentBackground);
}

template<class T>
QiliWidget<T>::~QiliWidget() {};

template<class T>
void QiliWidget<T>::mousePressEvent(QMouseEvent *event)
{
    QDialog::mousePressEvent(event);
    if (event->button() != Qt::LeftButton) {
        return;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    auto child = T::childAt(event->scenePosition().toPoint());
#else
    auto child = T::childAt(event->localPos().toPoint());
#endif
    bool should = child == nullptr;
    if (!should) {
        auto meta = child->metaObject();
        should = qstrcmp(meta->className(), "QWidget") == 0
                 || qstrcmp(meta->className(), "QiliTitleBar") == 0;
    }
    if (!should) {
        auto *parent = child->parentWidget();
        should = parent != nullptr && parent->property("qili-widget").toString() == "titlebar";
    }

    if (should) {
#if QILI_ON_WIN32
        // no need to change cursor
#else
        mSavedCursor = T::cursor();
        T::setCursor(Qt::SizeAllCursor);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        mStart = event->globalPosition();
#else
        mStart = event->globalPos();
#endif
        mPos = T::pos();
        qDebug() << "Move Start: @" << mPos;
    } else {
        qDebug() << "Found Child: " << child->metaObject()->className() << " => " << child->objectName();
    }
}

template<class T>
void QiliWidget<T>::mouseReleaseEvent(QMouseEvent *event)
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
    T::setCursor(mSavedCursor);
#endif
}

template<class T>
void QiliWidget<T>::mouseMoveEvent(QMouseEvent *event)
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
        T::move(mPos.x() + current.x() - mStart.x(), mPos.y() + current.y() - mStart.y());
    }
}

template<class T>
void QiliWidget<T>::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    auto brush = T::palette().brush(QPalette::Window);
    painter.setBrush(brush);

    QPainterPath path;
    auto rect = this->rect();
    path.addRoundedRect(rect, 5, 5);
    painter.drawPath(path);
    T::paintEvent(event);
}

template<class T>
void QiliWidget<T>::show()
{
    T::show();
    T::activateWindow();
    T::raise();
}

#endif // QILIWIDGET_H
