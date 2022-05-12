PROJECT   = Maths3D
TARGET    = Maths3D
VERSION   = 0.9.8
BRIEF     = "Maths for Computer Graphics"
LOGO      = docs/logo.svg
DOCS      = docs/README.md

SOURCES   = src/maths3d.cpp tests/tests.cpp examples/examples.pro
INCLUDES  = includes

LIBRARIES = m
CFLAGS    = -Wall -ffast-math -O2
CXXFLAGS  = -std=c++11 -fno-exceptions

MODULES   = https://github.com/JohnRyland/TestFramework.git

# Add some additional tests to check the generated code
.build/optimization_test.S: tests/optimization_test.cpp includes/maths3d.h
	$(CXX) $(CXX_FLAGS) $< -S -o $@

check_code_gen: .build/optimization_test.S
	@echo checking the compiler is optimizing to use inverse sqrt instruction
	grep rsqrtss .build/optimization_test.S > /dev/null

test: check_code_gen

