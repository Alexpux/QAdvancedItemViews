#-------------------------------------------------
#
# Project created by QtCreator 2012-01-20T23:03:15
#
#-------------------------------------------------

QT       += core gui

TARGET = example
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    spinboxitemdelegate.cpp

HEADERS  += mainwindow.h \
    spinboxitemdelegate.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    lorem_ipsum.txt

RESOURCES += \
    example.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qaivlib/release/ -lqaivlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qaivlib/debug/ -lqaivlib
else:symbian: LIBS += -lqaivlib
else:unix: LIBS += -L$$OUT_PWD/../qaivlib/ -lqaivlib

INCLUDEPATH += $$PWD/../qaivlib
DEPENDPATH += $$PWD/../qaivlib
