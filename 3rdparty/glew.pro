
GLEW_LIB=.build/install/usr/local/lib/libglew.a

$(GLEW_LIB):
	cd .modules/glew && make extensions
	cd .modules/glew && make glew.lib
	cd .modules/glew && GLEW_DEST=../../.build/install/usr/local GLEW_PREFIX=../../.build/install/usr/local make NAME=glew LIBDIR=../../.build/install/usr/local/lib install

build: $(GLEW_LIB)

