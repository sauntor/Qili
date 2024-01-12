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

#ifndef QILILOGGER_H
#define QILILOGGER_H

#include <QObject>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtLogging>
#else
#include <QtGlobal>
#endif
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QDate>

class QiliLogger : public QObject
{
    Q_OBJECT

public:
    void logging(QtMsgType type, const QMessageLogContext &context, const QString &message);

    static QiliLogger& install();


private:
    QiliLogger();
    ~QiliLogger();

    void console(QtMsgType type, const QMessageLogContext &context, const QString &message);
    QFile *log();

    bool              mInstalled{false};
    bool              mCanWrite{true};
    QtMessageHandler  mConsole{nullptr};
    QFile *mFile{nullptr};
    QDir  *mDir{nullptr};
    QDate  mDate{QDate::currentDate()};
};

#endif // QILILOGGER_H
