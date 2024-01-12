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

#include "QiliTitleBarDesigner.h"

#include "QiliTitleBar.h"

#include <QIcon>
#include <QString>
#include <QFile>

// using namespace Qt::StringLiterals;

QiliTitleBarDesigner::QiliTitleBarDesigner(QObject *parent)
    : QObject{parent}
{

}

bool QiliTitleBarDesigner::isContainer() const
{
    return false;
}

bool QiliTitleBarDesigner::isInitialized() const
{
    return initialized;
}

QIcon QiliTitleBarDesigner::icon() const
{
    return QIcon(":/images/qili.png");
}

QString QiliTitleBarDesigner::domXml() const
{


    return QString::fromUtf16(uR"(
<ui language="c++">
  <widget class="QiliTitleBar" name="qiliTitleBar">
)"
uR"(
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>400</width>
        <height>26</height>
      </rect>
    </property>
    <property name="qili-widget" stdset="0">
      <string notr="true">titlebar</string>
    </property>
")
uR"(
  </widget>
</ui>
)");
}

QString QiliTitleBarDesigner::group() const
{
    return QString::fromUtf16(u"Display Widgets");
}

QString QiliTitleBarDesigner::includeFile() const
{
    return QString::fromUtf16(u"QiliTitleBar.h");
}

QString QiliTitleBarDesigner::name() const
{
    return QString::fromUtf16(u"QiliTitleBar");
}

QString QiliTitleBarDesigner::toolTip() const
{
    return QString::fromUtf16(u"Qili Title Bar");
}

QString QiliTitleBarDesigner::whatsThis() const
{
    return QString::fromUtf16(u"Title Bar for Qili");
}

QWidget *QiliTitleBarDesigner::createWidget(QWidget *parent)
{
    auto *widget = new QiliTitleBar(parent);
    if (!styleSheet.isEmpty()) {
        widget->setStyleSheet(styleSheet);
    }
    return widget;
}

void QiliTitleBarDesigner::initialize(QDesignerFormEditorInterface *core)
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
