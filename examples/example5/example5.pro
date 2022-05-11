PROJECT  = example5
TARGET   = example5
SOURCES  = ../common/svg.cpp example5.cpp ../../src/maths3d.cpp
INCLUDES = ../../include ../common
CXXFLAGS = -std=c++11

OUTPUT   = example5.pdf

%.pdf: %.svg
	$(if $(shell which -a rsvg-convert),rsvg-convert --format pdf $< -o $@)

