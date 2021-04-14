QT += core
QT += network
QT -= gui

TARGET = Client
CONFIG += console
CONFIG += app_bundle

SOURCES += \
    ClientDriver.cpp \
    Environment.cpp \
    Client.cpp
    

HEADERS += \
    ../hdr/ClientDriver.h \
    ../hdr/Environment.h \
    ../hdr/Qt.h \
    ../hdr/Client.h
    

QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS += -lcrypto
QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -O0

LIBS += -fopenmp
LIBS += -lcrypto

DESTDIR=.
OBJECTS_DIR=../bin/
