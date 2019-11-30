/**
 * Copyright (c) 2011-2012 Martin Hoppe martin@2x2hoppe.de
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>
#include "mainwindow.h"

void loadTranslators()
{
    QTranslator* toolkitTranslator = new QTranslator(qApp);
    QTranslator* applicationTranslator = new QTranslator(qApp);

    if(toolkitTranslator->load(QLocale::system(), "qt", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        qApp->installTranslator(toolkitTranslator);
    }

    if(applicationTranslator->load(QLocale::system(), "aivlib", "_", QDir(QApplication::applicationDirPath()).filePath("translations"))) {
        qApp->installTranslator(applicationTranslator);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    loadTranslators();
    MainWindow w;
    w.show();

    return a.exec();
}
