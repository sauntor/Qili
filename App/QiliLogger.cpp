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

#include "QiliLogger.h"

#include <QDate>
#include <QDebug>

static const auto LOG_SUFFIX = ".log";

QiliLogger::QiliLogger() : QObject(nullptr)
{
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (dir.mkpath("logs")) {
        mDir = new QDir(dir.absoluteFilePath("logs"));
        mCanWrite = true;
    } else {
        mDir = nullptr;
        mCanWrite = false;
    }
    qSetMessagePattern("[%{type}]%{time}[%{qthreadptr}](%{file}:%{line}): %{message}");
}

QiliLogger::~QiliLogger()
{
    if (mFile != nullptr) {
        mFile->close();
        delete mFile;
    }
}

void QiliLogger::console(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    if (mConsole != nullptr) {
        mConsole(type, context, message);
    }
}

void QiliLogger::logging(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    if (!mCanWrite) {
        console(type, context, message);
        return;
    }
    auto file = log();
    if (file == nullptr) {
        return;
    }
    console(type, context, message);
    auto formated = qFormatLogMessage(type, context, message);
    file->write(qUtf8Printable(formated));
    file->write("\n");
    file->flush();
}

void QiliLogging(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    QiliLogger& logger = QiliLogger::install();
    logger.logging(type, context, message);
}


QiliLogger &QiliLogger::install()
{
    static QiliLogger logger;
    if (!logger.mInstalled) {
        logger.mConsole = qInstallMessageHandler(QiliLogging);
        logger.mInstalled = true;
        logger.clean();
        qInfo() << "QiliLogger initialized: has console? " << (logger.mConsole != nullptr);
    }
    return logger;
}

QString QiliLogger::dir()
{
    auto &logger = install();

    if (logger.mDir == nullptr || !logger.mDir->exists()) {
        return "";
    }
    return logger.mDir->canonicalPath();
}

QFile *QiliLogger::log()
{
    if (!mCanWrite) {
        return nullptr;
    }
    if (mDir == nullptr) {
        return nullptr;
    }
    QDate today = QDate::currentDate();
    if (mFile == nullptr || mDate != today) {
        if (mFile != nullptr) {
            mFile->close();
            delete mFile;
        }
        auto filename = today.toString(Qt::ISODate) + LOG_SUFFIX;
        auto file = new QFile(mDir->absoluteFilePath(filename));
        if (!file->open(QFile::ReadWrite)) {
            if (mConsole != nullptr) {
                mConsole(QtMsgType::QtCriticalMsg, QMessageLogContext(),
                         QString("Can't create or open log: %1s").arg(file->fileName()));
            }
            delete file;
            mFile = nullptr;
            mCanWrite = false;
        }
        else {
            mFile = file;
            mCanWrite = true;
            // remove expired logs
            clean();
        }
    }
    return mFile;
}

void QiliLogger::clean()
{
    if (!mCanWrite) {
        return;
    }

    const auto logs = mDir->entryList(QDir::Files);
    const auto today = QDate::currentDate();
    for (const auto &log : logs) {
        const auto dateString = log.sliced(0, log.size() - qstrlen(LOG_SUFFIX));
        const auto date = QDate::fromString(dateString, Qt::ISODate);
        // keep logs in a week
        if (date.daysTo(today) < 7) {
            continue;
        }
        auto removed = mDir->remove(log);
        if (!removed) {
            qCritical() << "Can NOT remove expired log: "
                        << mDir->absoluteFilePath(log);
        }
    }
}
