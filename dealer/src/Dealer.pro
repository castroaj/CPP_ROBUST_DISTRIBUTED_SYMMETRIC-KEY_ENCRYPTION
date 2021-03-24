QT = websockets

TARGET = Dealer
CONFIG += console
CONFIG += app_bundle

SOURCES += \
    DealerDriver.cpp \
    Environment.cpp \
    KeyGenerator.cpp \
    DealerServer.cpp
    

HEADERS += \
    ../hdr/DealerDriver.h \
    ../hdr/Environment.h \
    ../hdr/KeyGenerator.h \
    ../hdr/Qt.h \
    ../hdr/DealerServer.h
    

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