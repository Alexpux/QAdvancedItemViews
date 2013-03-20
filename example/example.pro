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
TARGET = example
QT += core gui
CONFIG += debug_and_release

CONFIG(debug, debug|release) {
    win32 {
        DESTDIR = ../bin/Win32/Debug
        LIBS += -L"../../qaivlib/debug" \
            -l$(DESTDIR)/qaivlib
    }
    INCLUDEPATH += ./debug \
        $(QTDIR)/mkspecs/default \
        ./GeneratedFiles \
        ./../qaivlib
    MOC_DIR += ./GeneratedFiles/debug
    OBJECTS_DIR += debug
} else {
    win32 {
        DESTDIR = ../bin/Win32/Release
        LIBS += -L"../../qaivlib/release" \
            -l$(DESTDIR)/qaivlib
    }
    INCLUDEPATH += ./release \
        $(QTDIR)/mkspecs/release \
        ./GeneratedFiles \
        ./../qaivlib
    MOC_DIR += ./GeneratedFiles/release
    OBJECTS_DIR += release
}

DEFINES += _WINDOWS QT_LARGEFILE_SUPPORT QT_DLL QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2
DEPENDPATH += .
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(example.pri)
