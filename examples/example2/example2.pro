PROJECT=example2
TARGET=example2

SOURCES=example2.cpp \
        ../common/bitmap.cpp \
        ../../src/maths3d.cpp \
				blah/blah.pro

# TODO: include paths should be relative to the .pro file
INCLUDES=includes
INCLUDES+=examples/common

CXXFLAGS=-std=c++11

