#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T18:31:15
#
#-------------------------------------------------

QT       += core gui network websockets widgets

TARGET = WatchDog
TEMPLATE = app

win32 {
    CONFIG(release, debug|release):QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}
DESTDIR = $$PWD/bin_vs

msvc:{
    target.path = $$PWD/bin_vs
}

INSTALLS += target

include(./Log/Log.pri)
include(./Utils/Utils.pri)

SOURCES += main.cpp \
    application.cpp \
    mainwindow.cpp \
    watchdogitem.cpp \
    watchdogmanager.cpp

HEADERS  += \
    application.h \
    cleanmem.h \
    mainwindow.h \
    watchdogitem.h \
    watchdogmanager.h

RESOURCES += \
    res.qrc

RC_ICONS=dog2.ico


FORMS += \
    mainwindow.ui

DISTFILES += \
    WatchDogManager.json

