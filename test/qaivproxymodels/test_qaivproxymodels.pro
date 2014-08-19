#/******************************************************************************
#** This file is part of qadvanceditemviews.
#**
#** Copyright (c) 2011-2013 Martin Hoppe martin@2x2hoppe.de
#**
#** qadvanceditemviews is free software: you can redistribute it
#** and/or modify it under the terms of the GNU Lesser General
#** Public License as published by the Free Software Foundation,
#** either version 3 of the License, or (at your option) any
#** later version.
#**
#** qadvanceditemviews is distributed in the hope that it will be
#** useful, but WITHOUT ANY WARRANTY; without even the implied
#** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#** See the GNU General Public License for more details.
#**
#** You should have received a copy of the GNU Lesser General Public
#** License along with qadvanceditemviews.
#** If not, see <http://www.gnu.org/licenses/>.
#******************************************************************************/

TEMPLATE = app
TARGET = test_qaivproxymodels

equals(QT_MAJOR_VERSION, 4){
    CONFIG += qtestlib console debug_and_release
    QT += core gui
}
equals(QT_MAJOR_VERSION, 5){
    CONFIG += console debug_and_release
    QT += core widgets testlib
}
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
    win32 {
        DESTDIR = ../../bin/Win32/Debug
        LIBS += -L../../bin/Win32/Debug -lqaivlib
    }
    else {
        DESTDIR = ../../Debug
        LIBS += -L./../../Debug -lqaivlib
    }
    INCLUDEPATH += . \
        ./GeneratedFiles/Debug \
        ./../../qaivlib
    MOC_DIR += ./GeneratedFiles/Debug
    OBJECTS_DIR += Debug
} else {
    win32 {
        DESTDIR = ../../bin/Win32/Release
        LIBS += -l$(DESTDIR)/qaivlib
    }
    else {
        DESTDIR = ../../Release
        LIBS += -L./../../Release -lqaivlib
    }
    INCLUDEPATH += . \
        ./GeneratedFiles/Release \
        ./../../qaivlib
    MOC_DIR += ./GeneratedFiles/Release
    OBJECTS_DIR += Release
}

win32 {
    DEFINES += QT_LARGEFILE_SUPPORT QT_DLL
}
DEPENDPATH += .
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(test_qaivproxymodels.pri)
