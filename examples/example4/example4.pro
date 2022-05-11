PROJECT=example4
TARGET=example4
SOURCES=../common/svg.cpp example4.cpp ../../src/maths3d.cpp
INCLUDES=../../includes ../common
CXXFLAGS=-std=c++11

OUTPUT=example4.pdf

%.pdf: %.svg
	$(if $(shell which -a rsvg-convert),rsvg-convert --format pdf $< -o $@)

