TEMPLATE = lib
TARGET = TCP

QT += network core

CONFIG += c++17
CONFIG += plugin

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

INCLUDEPATH += ../headers
INCLUDEPATH += ../../ComponentMatrix/src/

SOURCES += \
    TCPServer.cpp \
    TCPClient.cpp \
    TCPProto.cpp

HEADERS +=\
    ../headers/TCPServer.hpp \
    ../headers/TCPClient.hpp \
    ../headers/TCPProto.hpp \
    ../headers/Constants.hpp
