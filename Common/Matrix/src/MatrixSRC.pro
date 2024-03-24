TEMPLATE = lib
TARGET = Matrix

QT += widgets \
    core \ 
    gui \

SOURCES += matrix.cpp 

CONFIG += plugins

HEADERS += matrix.h \
           Matrix_global.h
    
