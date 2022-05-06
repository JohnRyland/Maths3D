PROJECT  = example5
TARGET   = example5
SOURCES  = ../common/svg.cpp example5.cpp ../../src/maths3d.cpp
INCLUDES = ../../includes ../common
CXXFLAGS = -std=c++11

OUTPUT   = cube.pdf

cube.pdf: cube.svg
	rsvg-convert --format pdf $< -o $@

