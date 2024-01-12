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

#ifndef QILITEXTFIELD_H
#define QILITEXTFIELD_H

#include "WidgetsGlobal.h"

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

class QILI_WIDGETS_EXPORT QiliTextField : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QLineEdit::EchoMode echoMode READ echoMode WRITE setEchoMode FINAL)
    Q_PROPERTY(QString label READ label WRITE setLabel FINAL)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(QString placeholder READ placeholder WRITE setPlaceholder FINAL)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment FINAL)

public:
    explicit QiliTextField(QWidget *parent = nullptr);

    void setLabel(const QString &label);
    QString label() const;

    void setText(const QString &text);
    QString text() const;

    void setEchoMode(QLineEdit::EchoMode value);
    QLineEdit::EchoMode echoMode() const;

    void setPlaceholder(const QString &text);
    QString placeholder() const;

    void setAlignment(Qt::Alignment alignment);
    Qt::Alignment alignment() const;

signals:
    void textChanged(QString text);

public slots:
    void clear();

protected:
    void resizeEvent(QResizeEvent *) override;

private slots:
    void onClearClicked();
    void onTextChanged(const QString &text);

private:
    QLineEdit *mText;
    QLineEdit::EchoMode mEchoMode = QLineEdit::Normal;
    QLabel    *mLabel;
    QPushButton *mClear;
    QHBoxLayout *mLayout;
};

#endif // QILITEXTFIELD_H
