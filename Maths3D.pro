PROJECT   = Maths3D
TARGET    = Maths3D
SOURCES   = tests.cpp
DOCS      = README.md
LIBRARIES = m
CFLAGS    = -Wall -ffast-math -O2
CXXFLAGS  = -std=c++11 -fno-exceptions
MODULES   = https://github.com/JohnRyland/TestFramework.git

# Add some additional tests to check the generated code
optimization_test.S: optimization_test.cpp maths3d.h
	$(CXX) -ffast-math -std=c++11 -O2 $< -S -o $@

check_code_gen: optimization_test.S
	@echo checking the compiler is optimizing to use inverse sqrt instruction
	grep rsqrtss optimization_test.S > /dev/null

test: check_code_gen

