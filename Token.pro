#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T16:26:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += "/usr/lib/viettel-ca_v4.so"

LIBS += -ldl\
        -lcrypto

TARGET = Token
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pkcs11.cpp

HEADERS  += mainwindow.h \
    pkcs11.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc


