ifeq ($(PLATFORM),Windows)
  GENERATOR=-G "MinGW Makefiles"
endif

GLFW_LIB=$(OUTPUT_DIR)/install/usr/local/lib/libglfw3.a

$(GLFW_LIB):
	if [ ! -d $(OUTPUT_DIR)/glfw ] ; then  mkdir -p $(OUTPUT_DIR)/glfw && cd $(OUTPUT_DIR)/glfw && cmake ../../../.modules/glfw $(GENERATOR); fi
	cd $(OUTPUT_DIR)/glfw && cmake --build . --config Release && cmake --install . --prefix ../install/usr/local

build: $(GLFW_LIB)

