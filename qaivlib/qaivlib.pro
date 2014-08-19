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

TEMPLATE = lib
TARGET = qaivlib

equals(QT_MAJOR_VERSION, 4){
    QT += core gui
}
equals(QT_MAJOR_VERSION, 5){
    QT += core widgets
}

CONFIG += debug_and_release

unix {
    DEFINES += QAIVLIB_LIBRARY
}
else {
    DEFINES += QT_LARGEFILE_SUPPORT QT_DLL QAIVLIB_LIBRARY
}

CONFIG(debug, debug|release) {
    win32 {
        DESTDIR = ./../bin/Win32/Debug
    }
    else {
        DESTDIR = ./../Debug
    }
    INCLUDEPATH += ./GeneratedFiles \
        . \
        ./GeneratedFiles/Debug
    MOC_DIR += ./GeneratedFiles/debug
    OBJECTS_DIR += debug
} else {
    win32 {
        DESTDIR = ./../bin/Win32/Release
    }
    else {
        DESTDIR = ./../Release
    }
    INCLUDEPATH += ./GeneratedFiles \
        . \
        ./GeneratedFiles/Release
    MOC_DIR += ./GeneratedFiles/Release
    OBJECTS_DIR += Release
}


PRECOMPILED_HEADER = stdafx.h
DEPENDPATH += .
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(qaivlib.pri)
