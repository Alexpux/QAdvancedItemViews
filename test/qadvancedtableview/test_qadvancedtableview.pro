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
TARGET = test_qadvancedtableview

equals(QT_MAJOR_VERSION, 5){
    CONFIG += debug console
    CONFIG -= app_bundle
    QT += testlib widgets
}

equals(QT_MAJOR_VERSION, 4){
    CONFIG += qtestlib debug console
    CONFIG -= app_bundle
    QT += core gui

}

CONFIG(debug, debug|release) {
    win32 {
        DESTDIR = ../../bin/Win32/Debug
        DEFINES += QT_LARGEFILE_SUPPORT QT_DLL
        DESTDIR = ../Debug
        LIBS += -L"../../bin/Win32/Debug" -lqaivlib
    }
    else {
        DESTDIR = ../../Debug
        LIBS += -L./../../Debug -lqaivlib
    }

    MOC_DIR += ./GeneratedFiles/Debug
    INCLUDEPATH += ./GeneratedFiles/Debug
    OBJECTS_DIR += Debug
} else {
    win32 {
        DESTDIR = ../../bin/Win32/Release
        DEFINES += QT_LARGEFILE_SUPPORT QT_DLL
        DESTDIR = ../Debug
        LIBS += -L"../../bin/Win32/Release" -lqaivlib
    }
    else {
        DESTDIR = ../../Release
        LIBS += -L./../../Release -lqaivlib
    }

    MOC_DIR += ./GeneratedFiles/Release
    INCLUDEPATH += ./GeneratedFiles/Release
    OBJECTS_DIR += Release
}

INCLUDEPATH += . \
    ./../../qaivlib

DEPENDPATH += .
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

include(test_qadvancedtableview.pri)
