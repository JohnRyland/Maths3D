PROJECT   = Maths3D
BRIEF     = "Maths for Computer Graphics"
LOGO      = docs/logo.svg
TARGET    = Maths3D
SOURCES   = tests/tests.cpp
INCLUDES  = includes
DOCS      = docs/README.md
LIBRARIES = m
CFLAGS    = -Wall -ffast-math -O2
CXXFLAGS  = -std=c++11 -fno-exceptions
MODULES   = https://github.com/JohnRyland/TestFramework.git

# TODO: GenericMake and modules should be able to define exports and then when
# a project adds a module, automatically the exports are imported / appended
# to the project settings (instead of like here where we are doing it like this)
INCLUDES += .modules/TestFramework/include
SOURCES  += .modules/TestFramework/src/TestMain.cpp

# Add some additional tests to check the generated code
.build/optimization_test.S: tests/optimization_test.cpp includes/maths3d.h
	$(CXX) $(CXX_FLAGS) $< -S -o $@

check_code_gen: .build/optimization_test.S
	@echo checking the compiler is optimizing to use inverse sqrt instruction
	grep rsqrtss .build/optimization_test.S > /dev/null

test: check_code_gen

