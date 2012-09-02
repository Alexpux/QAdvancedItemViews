#-------------------------------------------------
#
# Project created by QtCreator 2012-05-30T20:43:08
#
#-------------------------------------------------

TARGET = qaivlib
TEMPLATE = lib

DEFINES += QAIVLIB_LIBRARY

SOURCES += qaiv.cpp \
    stdafx.cpp \
    qabstractfilter.cpp \
    qadvancedtableview.cpp \
    qfilterviewconnector.cpp \
    qadvancedheaderview.cpp \
    qfilterview.cpp \
    qfiltermodel.cpp \
    qfiltergroup.cpp \
    qfiltermodelproxy.cpp \
    qclickablelabel.cpp \
    qheaderviewconnector.cpp \
    qabstractmodelitemdecoration.cpp \
    qabstractfilterproxymodel.cpp \
    qabstractfiltermodel.cpp \
    qautofilter.cpp \
    qregexpfilter.cpp \
    qrangefilter.cpp \
    qvaluefilter.cpp \
    qtextfilter.cpp \
    qcheckstateproxymodel.cpp \
    qsinglecolumnproxymodel.cpp \
    qfilterviewitemdelegate.cpp \
    qselectionlistfilter.cpp \
    qgroupingproxymodel.cpp \
    qconditionaldecorationdialog.cpp \
    qconditionaldecoration.cpp \
    qconditionaldecorationproxymodel.cpp \
    quniquevaluesproxymodel.cpp \
    qfixedrowstableview.cpp \
    qshareditemselectionmodel.cpp \
    qfixedrowsheaderview.cpp \
    qfiltereditorwidget.cpp \
    qfiltereditorpopupwidget.cpp

HEADERS += qaiv.h\
        qaivlib_global.h \
    stdafx.h \
    qabstractfilter.h \
    qadvancedtableview_p.h \
    qadvancedtableview.h \
    qfilterviewconnector.h \
    qadvancedheaderview.h \
    qfilterview.h \
    qfiltermodel.h \
    qfiltergroup.h \
    qfiltermodelproxy.h \
    qheaderviewconnector.h \
    qgroupingproxymodel_p.h \
    qgroupingproxymodel.h \
    qabstractmodelitemdecoration.h \
    qabstractfilterproxymodel.h \
    qabstractfiltermodel.h \
    qautofilter_p.h \
    qautofilter.h \
    qregexpfilter_p.h \
    qregexpfilter.h \
    qrangefilter_p.h \
    qrangefilter.h \
    qvaluefilter_p.h \
    qvaluefilter.h \
    qtextfilter_p.h \
    qtextfilter.h \
    qcheckstateproxymodel.h \
    qsinglecolumnproxymodel.h \
    qfilterviewitemdelegate.h \
    qclickablelabel.h \
    qselectionlistfilter_p.h \
    qselectionlistfilter.h \
    qconditionaldecorationdialog_p.h \
    qconditionaldecorationdialog.h \
    qconditionaldecoration.h \
    qconditionaldecorationproxymodel.h \
    quniquevaluesproxymodel.h \
    qfixedrowstableview.h \
    qshareditemselectionmodel.h \
    qfixedrowsheaderview.h \
    qfiltereditorwidget.h \
    qfiltereditorpopupwidget.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEAC32228
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qaivlib.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    qadvancedtableview.ui \
    qconditionaldecorationdialog.ui

RESOURCES += \
    qaiv.qrc
