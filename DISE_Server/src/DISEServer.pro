QT += core
QT += network
QT -= gui

TARGET = DiseServer
CONFIG += console
CONFIG += app_bundle

SOURCES += \
    DISEServerDriver.cpp \
    Environment.cpp \
    DISEServer.cpp
    

HEADERS += \
    ../hdr/DISEServerDriver.h \
    ../hdr/Environment.h \
    ../hdr/Qt.h \
    ../hdr/DISEServer.h
    

QMAKE_CXXFLAGS += -fopenmp
QMAKE_CXXFLAGS += -lcrypto
QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -O0

LIBS += -fopenmp
LIBS += -lcrypto

DESTDIR=.
OBJECTS_DIR=../bin/

target.path = $$[QT_INSTALL_EXAMPLES]/websockets/echoclient
INSTALLS += target