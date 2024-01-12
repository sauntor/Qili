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

#include "QiliApp.h"

#include "QiliGlobal.h"
#include "QiliTray.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QEventLoop>
#include <QWebSocket>
#include <QSizePolicy>
#include <QSettings>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QOperatingSystemVersion>

using namespace Qili;

int QiliApp(QApplication &app)
{
    app.setApplicationVersion(ApplicationVersion);
    app.setApplicationName(ApplicationName);
    app.setOrganizationName(OrganizationName);
    app.setOrganizationDomain(OrganizationDomain);

    QDir binDir = QCoreApplication::applicationDirPath();
    QDir appDir = binDir.absoluteFilePath("..");

    QTranslator qtranslator;
    if (qtranslator.load(QLocale::system(), QString("qt"), QString("_"),
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                         QLibraryInfo::path(QLibraryInfo::TranslationsPath))
#else
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath))
#endif
        ) {
        app.installTranslator(&qtranslator);
    } else {
        qWarning() << "Can't load Qt Library Translations";
    }

    const QStringList uiLanguages = QLocale::system().uiLanguages();
    const QStringList components = {"App", "Widgets"};
    for (const auto &component : std::as_const(components)) {
        QTranslator *translator = new QTranslator(&app);
        auto found = false;
        for (const QString &locale : uiLanguages) {
            const QString baseName = "Qili" + component + "_" + QLocale(locale).name();
            if (translator->load(":/i18n/" + baseName) ||
                // For Debug Environment
                translator->load(appDir.filePath(component + "/") + baseName) ||
                // For Release Environment
                translator->load(appDir.filePath("share/translations/") + baseName)) {
                qDebug() << "translation loaded: " << translator->filePath();
                found = true;
                break;
            }
        }
        if (found) {
            app.installTranslator(translator);
        } else {
            delete translator;
        }
    }


    QFile theme(":/themes/light.css");
    if (theme.open(QFile::ReadOnly)) {
        auto styles = theme.readAll();
        app.setStyleSheet(styles);
    }
    app.setQuitOnLastWindowClosed(false);


    QiliTray tray;
    tray.show();


    return app.exec();
}
