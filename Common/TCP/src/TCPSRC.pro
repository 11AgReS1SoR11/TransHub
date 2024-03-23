TEMPLATE = lib
TARGET = TCP

QT += network core

CONFIG += c++14
CONFIG += plugin

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

INCLUDEPATH += ../headers

SOURCES += \
    TCPServer.cpp \
    TCPClient.cpp \

HEADERS +=\
    ../headers/TCPServer.hpp \
    ../headers/TCPClient.hpp \
    ../headers/Constants.hpp
