PROJECT=example3
TARGET=example3

SOURCES=example3.cpp \
        ../common/bitmap.cpp \
        ../../src/maths3d.cpp

# TODO: include paths should be relative to the .pro file
INCLUDES=../../includes
INCLUDES+=../common

CXXFLAGS=-std=c++11

