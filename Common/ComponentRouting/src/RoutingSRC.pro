TEMPLATE = lib
TARGET = $$qtLibraryTarget(ComponentRouting)

QT += core

CONFIG += c++17
CONFIG += plugin

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

INCLUDEPATH += ../headers

SOURCES += Routing.cpp

HEADERS +=\
    ../headers/Routing.hpp
    ../headers/Constants.hpp
