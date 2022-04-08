#  
#  Copyright (c) 2021, John Ryland
#  All rights reserved.
#  
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  
#  1. Redistributions of source code must retain the above copyright notice, this
#     list of conditions and the following disclaimer. 
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#  
#  The views and conclusions contained in the software and documentation are those
#  of the authors and should not be interpreted as representing official policies, 
#  either expressed or implied, of the cgMaths Project.
# 


all: run_tests


run_tests: .tags tests optimization_test.S
	@echo checking the compiler is optimizing to use inverse sqrt instruction
	grep rsqrtss optimization_test.S > /dev/null
	./tests


debug: .tags tests_d
	./tests_d


clean:
	rm -rf tests tests_d tests_d.dSYM .tags optimization_test.S


tests_d: tests.cpp maths3d.h maths3d_ext.h
	$(CXX) -g -std=c++11 $< -o $@ -lm


tests: tests.cpp maths3d.h maths3d_ext.h
	$(CXX) -ffast-math -std=c++11 -O2 $< -o $@ -lm


optimization_test.S: optimization_test.cpp maths3d.h
	$(CXX) -ffast-math -std=c++11 -O2 $< -S -o $@


.tags: $(patsubst %, ./%, *.cpp *.h)
	ctags --tag-relative=yes --c++-kinds=+pl --fields=+iaS --extra=+q --language-force=C++ -f $@ $^ 2> /dev/null ; true


