#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T18:31:15
#
#-------------------------------------------------

QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LIBS_App_ServerWatchDog
TEMPLATE = app

win32 {
    CONFIG(release, debug|release):QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}
DESTDIR = $$PWD/../bin_vs

mingw:{
    DEFINES += USING_QT_MINGW
    target.path = $$PWD/bin_mingw
}

msvc:{
    target.path = $$PWD/bin_vs
}

INSTALLS += target


SOURCES += main.cpp \
    application.cpp

HEADERS  += \
    application.h \
    cleanmem.h

RESOURCES += \
    res.qrc

CONFIG(debug, debug|release): VERSION = 1.0.2.1
CONFIG(release, debug|release) : VERSION = 1.0.2.0

