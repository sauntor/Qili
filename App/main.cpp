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

#include "QiliGlobal.h"
#include "QiliLogger.h"

#include <QLibrary>
#include <QApplication>
#include <QDir>
#include <QDebug>

using QiliAppType = int (*)(QApplication &app);

QStringList libsForQili();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QiliLogger::install();
    qDebug() << "Starting...";

    QDir dir = app.applicationDirPath();
    dir.cdUp();

    const auto libs = libsForQili();
    for (const auto &lib : std::as_const(libs)) {
        auto path = dir.absoluteFilePath(lib);
        if (QFile(path).exists()) {
            QCoreApplication::addLibraryPath(QString(path));
            qDebug() << "Added to Library Path: " << path;
        }
    }
    const auto libraryPaths = QCoreApplication::libraryPaths();
    qDebug() << "Final libraryPaths = " << libraryPaths;

    QiliAppType QiliApp = nullptr;
    const auto modules = { "QRCodeGen", "QiliWidgets", "QiliApp" };
    for (const auto &module : modules) {
        for (const auto &path : std::as_const(libraryPaths)) {
            QLibrary lib(path + QDir::separator() + module);
            if (lib.load()) {
                qDebug() << "Loaded Module = " << module << " => " << lib.fileName();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                if (std::strcmp(module, "QiliApp") == 0) {
#else
                if (strcmp(module, "QiliApp") == 0) {
#endif
                    QiliApp = (QiliAppType) lib.resolve("QiliApp");
                }
                break;
            } else {
                qDebug() << "Can't load : " << module << " => " << lib.errorString();
            }
        }
    }
    if (QiliApp == nullptr) {
        qCritical() << "QiliApp not found in: " << libraryPaths;
        return -1;
    }

    qDebug() << "Started";

    int code = 0;
    do {
        code = QiliApp(app);
    } while (code == Qili::Restart);

    return code;
}

QStringList libsForQili()
{
    if (Qili::Released) {
        return { "lib64", "lib", "plugins" };
    }
    else {
        // for development layout
        return {"Thirdparty", "Widgets", "App"};
    }
}
