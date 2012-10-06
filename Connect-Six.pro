#-----------------------------------------------------------
#
# Project created by QtCreator 2012-09-15T19:32:03
#
#-----------------------------------------------------------

QT       += core gui

TARGET = Connect-Six
TEMPLATE = app


SOURCES += main.cpp\
        game_main.cpp \
    playchess.cpp \
    checkifwin.cpp \
    tds.cpp

HEADERS  += game_main.h \
    playchess.h \
    checkifwin.h \
    tds.h

FORMS    += game_main.ui

RESOURCES += \
    connect-six.qrc

OTHER_FILES += \
    readme.txt







