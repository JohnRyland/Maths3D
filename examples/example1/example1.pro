PROJECT=example1
TARGET=example1

SOURCES=example1.cpp \
        ../common/bitmap.cpp \
        ../../src/maths3d.cpp

# TODO: include paths should be relative to the .pro file
INCLUDES=includes
INCLUDES+=examples/common

CXXFLAGS=-std=c++11

