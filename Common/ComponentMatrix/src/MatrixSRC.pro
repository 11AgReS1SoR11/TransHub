TEMPLATE = lib
TARGET = $$qtLibraryTarget(ComponentMatrix)

SOURCES += matrix.cpp 

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

CONFIG += c++14
CONFIG += plugin

HEADERS += Matrix.hpp \
           Matrix_global.h
    
