
GLEW_LIB=$(OUTPUT_DIR)/install/usr/local/lib/libglew.a

$(GLEW_LIB):
	cd .modules/glew && make extensions
	cd .modules/glew && make glew.lib
	cd .modules/glew && GLEW_DEST=../../$(OUTPUT_DIR)/install/usr/local GLEW_PREFIX=../../$(OUTPUT_DIR)/install/usr/local make NAME=glew LIBDIR=../../$(OUTPUT_DIR)/install/usr/local/lib install

build: $(GLEW_LIB)

