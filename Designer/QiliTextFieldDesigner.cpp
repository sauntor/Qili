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

#include "QiliTextFieldDesigner.h"

#include "QiliTextField.h"

#include <QIcon>
#include <QString>
#include <QFile>

// using namespace Qt::StringLiterals;

QiliTextFieldDesigner::QiliTextFieldDesigner(QObject *parent)
    : QObject{parent}
{}

bool QiliTextFieldDesigner::isContainer() const
{
    return false;
}

bool QiliTextFieldDesigner::isInitialized() const
{
    return initialized;
}

QIcon QiliTextFieldDesigner::icon() const
{
    return QIcon(":/images/qili.png");
}

QString QiliTextFieldDesigner::domXml() const
{
    return QString::fromUtf16(uR"(
<ui language="c++">
  <widget class="QiliTextField" name="qiliTextField">
)"
uR"(
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>200</width>
        <height>31</height>
      </rect>
    </property>
    <property name="qili-widget" stdset="0">
      <string notr="true">text-field</string>
    </property>
")
uR"(
  </widget>
</ui>
)");
}

QString QiliTextFieldDesigner::group() const
{
    return QString::fromUtf16(u"Display Widgets");
}

QString QiliTextFieldDesigner::includeFile() const
{
    return QString::fromUtf16(u"QiliTextField.h");
}

QString QiliTextFieldDesigner::name() const
{
    return QString::fromUtf16(u"QiliTextField");
}

QString QiliTextFieldDesigner::toolTip() const
{
    return QString::fromUtf16(u"Qili Text Field");
}

QString QiliTextFieldDesigner::whatsThis() const
{
    return QString::fromUtf16(u"Text Field for Qili");
}

QWidget *QiliTextFieldDesigner::createWidget(QWidget *parent)
{
    QiliTextField *widget = new QiliTextField(parent);
    if (!styleSheet.isEmpty()) {
        widget->setStyleSheet(styleSheet);
    }
    return widget;
}

void QiliTextFieldDesigner::initialize(QDesignerFormEditorInterface *core)
{
    if (initialized) {
        return;
    }
    QFile file(":/themes/light.css");
    if (file.open(QFile::ReadOnly)) {
        styleSheet = file.readAll();
        file.close();
    }
    initialized = true;
}
