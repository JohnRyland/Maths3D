ifeq ($(PLATFORM),Windows)
  GENERATOR=-G "MinGW Makefiles"
endif

GLFW_LIB=.build/install/usr/local/lib/libglfw3.a

$(GLFW_LIB):
	if [ ! -d .build/glfw ] ; then  mkdir -p .build/glfw && cd .build/glfw && cmake ../../.modules/glfw $(GENERATOR); fi
	cd .build/glfw && cmake --build . --config Release && cmake --install . --prefix ../install/usr/local

build: $(GLFW_LIB)

