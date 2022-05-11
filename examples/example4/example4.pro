PROJECT   = example4
TARGET    = example4
SOURCES   = ../common/svg.cpp example4.cpp ../../src/maths3d.cpp
INCLUDES  = ../../include ../common
CXXFLAGS  = -std=c++11

OUTPUT    = example4.pdf

%.pdf: %.svg
	rsvg-convert --format pdf $< -o $@

