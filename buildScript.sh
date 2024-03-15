#!/bin/bash

# Creating the build directory (if it doesn't exist)
mkdir -p build

# Changing to the build directory
cd build

# Running qmake to generate the Makefile
qmake ../TransHub.pro

# Building the project using make
make
