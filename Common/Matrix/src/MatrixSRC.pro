TEMPLATE = lib
TARGET = Matrix

QT += widgets \
    core \ 
    gui \

SOURCES += matrix.cpp

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

CONFIG += c++14
CONFIG += plugins

HEADERS += matrix.h \
           Matrix_global.h
    
