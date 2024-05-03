TEMPLATE = lib
TARGET = CommonStructures

QT += core

CONFIG += c++17
CONFIG += plugin

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

INCLUDEPATH += headers
INCLUDEPATH += ../ComponentMatrix/src/
INCLUDEPATH += ../CommonStructures

SOURCES += \
    src/Coordinate.cpp

HEADERS +=\
    headers/Coordinate.hpp
