include 3rdparty/glew.pro
include 3rdparty/glfw.pro

CFLAGS += -I$(OUTPUT_DIR)/install/usr/local/include -DGLEW_STATIC

# Platform specific OpenGL libraries
ifeq ($(PLATFORM),Windows)
  GL_LFLAGS := -lopengl32 -lglu32 -lkernel32 -lgdi32 -luser32
endif
ifeq ($(PLATFORM),Darwin)
  GL_LFLAGS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework QuartzCore
endif
ifeq ($(PLATFORM),Linux)
  GL_LFLAGS := -lGL -lGLU -ldl -lm -pthread
endif

bin/file_browser: .modules/Nuklear/example/file_browser.c $(GLEW_LIB) $(GLFW_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(GL_LFLAGS)

bin/extended: .modules/Nuklear/example/extended.c $(GLEW_LIB) $(GLFW_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(GL_LFLAGS)

bin/canvas: .modules/Nuklear/example/canvas.c $(GLEW_LIB) $(GLFW_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(GL_LFLAGS)

bin/skinning: .modules/Nuklear/example/skinning.c $(GLEW_LIB) $(GLFW_LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(GL_LFLAGS)

build: bin/file_browser bin/extended bin/canvas bin/skinning

