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

include(../qaiv.pri)

TEMPLATE = lib

TARGET = $${QAIVLIB}

QT += core widgets

CONFIG += debug_and_release

DEFINES += QAIVLIB_LIBRARY

HEADERS += qabstractfilter.h \
           qabstractfiltermodel.h \
           qabstractfilterproxymodel.h \
           qabstractmodelitemdecoration.h \
           qadvancedheaderview.h \
           qadvancedheaderview_p.h \
           qadvancedtableview.h \
           qadvancedtableview_p.h \
           qaiv.h \
           qaivlib_global.h \
           qautofilter.h \
           qautofilter_p.h \
           qcheckstateproxymodel.h \
           qclickablelabel.h \
           qconditionaldecoration.h \
           qconditionaldecorationdialog.h \
           qconditionaldecorationdialog_p.h \
           qconditionaldecorationproxymodel.h \
           qfiltereditorpopupwidget.h \
           qfiltereditorwidget.h \
           qfiltergroup.h \
           qfiltermodel.h \
           qfiltermodelproxy.h \
           qfilterview.h \
           qfilterviewconnector.h \
           qfilterviewitemdelegate.h \
           qfixedrowsheaderview.h \
           qfixedrowstableview.h \
           qgroupingproxymodel.h \
           qgroupingproxymodel_p.h \
           qheaderviewconnector.h \
           qmimedatautil.h \
           qrangefilter.h \
           qrangefilter_p.h \
           qregexpfilter.h \
           qregexpfilter_p.h \
           qsearchbar.h \
           qsearchbar_p.h \
           qselectionlistfilter.h \
           qselectionlistfilter_p.h \
           qshareditemselectionmodel.h \
           qsinglecolumnproxymodel.h \
           qtablemodelcsvwriter_p.h \
           qtablemodelexcelmlwriter_p.h \
           qtablemodelexportpushbutton.h \
           qtablemodelhtmlwriter_p.h \
           qtablemodelwordmlwriter_p.h \
           qtablemodelwriter.h \
           qtextfilter.h \
           qtextfilter_p.h \
           quniquevaluesproxymodel.h \
           qvaluefilter.h \
           qvaluefilter_p.h

SOURCES += qabstractfilter.cpp \
           qabstractfiltermodel.cpp \
           qabstractfilterproxymodel.cpp \
           qabstractmodelitemdecoration.cpp \
           qadvancedheaderview.cpp \
           qadvancedtableview.cpp \
           qaiv.cpp \
           qautofilter.cpp \
           qcheckstateproxymodel.cpp \
           qclickablelabel.cpp \
           qconditionaldecoration.cpp \
           qconditionaldecorationdialog.cpp \
           qconditionaldecorationproxymodel.cpp \
           qfiltereditorpopupwidget.cpp \
           qfiltereditorwidget.cpp \
           qfiltergroup.cpp \
           qfiltermodel.cpp \
           qfiltermodelproxy.cpp \
           qfilterview.cpp \
           qfilterviewconnector.cpp \
           qfilterviewitemdelegate.cpp \
           qfixedrowsheaderview.cpp \
           qfixedrowstableview.cpp \
           qgroupingproxymodel.cpp \
           qheaderviewconnector.cpp \
           qmimedatautil.cpp \
           qrangefilter.cpp \
           qregexpfilter.cpp \
           qsearchbar.cpp \
           qselectionlistfilter.cpp \
           qshareditemselectionmodel.cpp \
           qsinglecolumnproxymodel.cpp \
           qtablemodelcsvwriter.cpp \
           qtablemodelexcelmlwriter.cpp \
           qtablemodelexportpushbutton.cpp \
           qtablemodeltmlwriter.cpp \
           qtablemodelwordmlwriter.cpp \
           qtablemodelwriter.cpp \
           qtextfilter.cpp \
           quniquevaluesproxymodel.cpp \
           qvaluefilter.cpp

FORMS += qadvancedtableview.ui \
         qconditionaldecorationdialog.ui \
         qsearchbar.ui

TRANSLATIONS += qaivlib_de.ts aivlib_ru.ts

RESOURCES += qaiv.qrc
