
LIB=.build/install/usr/local/lib/libglfw3.a

$(LIB):
	if [ ! -d .build/glfw ] ; then  mkdir -p .build/glfw && cd .build/glfw && cmake ../../.modules/glfw ; fi
	cd .build/glfw && cmake --build . && cmake --install . --prefix ../install/usr/local

build: $(LIB)

