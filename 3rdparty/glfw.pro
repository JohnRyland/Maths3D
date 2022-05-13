
LIB=.build/install/usr/local/lib/libglfw3.a

$(LIB):
	mkdir -p .build/glfw && cd .build/glfw && cmake ../../.modules/glfw && \
		cmake --build . && cmake --install . --prefix ../install/usr/local

build: $(LIB)

