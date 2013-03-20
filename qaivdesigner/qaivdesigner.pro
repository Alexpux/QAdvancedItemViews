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
TARGET = qaivdesigner

QT += core gui xml script
CONFIG += designer plugin

CONFIG(debug, debug|release) {
    win32 {
        DESTDIR = ../bin/Win32/Debug/plugins/designer
        INCLUDEPATH += ./../qaivlib \
            ./debug \
            $(QTDIR)/mkspecs/win32-msvc2008
        LIBS += -L"../../qaivlib/debug" \
            -l../bin/Win32/Debug/qaivlib
    }
    MOC_DIR += debug
    OBJECTS_DIR += debug
} else {
    win32 {
        DESTDIR = ../bin/Win32/Release/plugins/designer
        INCLUDEPATH += ./../qaivlib \
            ./release \
            $(QTDIR)/mkspecs/win32-msvc2008
        LIBS += -L"../../qaivlib/release" \
            -l../bin/Win32/Release/qaivlib
    }
    MOC_DIR += release
    OBJECTS_DIR += release
}


DEFINES += _WINDOWS QT_LARGEFILE_SUPPORT QT_DLL QT_SCRIPT_LIB QT_XML_LIB QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2

DEPENDPATH += .
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(qaivdesigner.pri)

#Install the plugin in the designer plugins directory.
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
