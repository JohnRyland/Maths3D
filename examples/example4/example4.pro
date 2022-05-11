PROJECT=example4
TARGET=example4
SOURCES=../common/svg.cpp example4.cpp ../../src/maths3d.cpp
INCLUDES=../../includes ../common
CXXFLAGS=-std=c++11

OUTPUT=cube.pdf

cube.pdf: cube.svg
	$(if $(shell which -a rsvg-convert),rsvg-convert --format pdf $< -o $@)



