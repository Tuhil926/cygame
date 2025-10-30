all: build

CYGAME_SRC_FILES := cygame.cpp Shape.cpp ShapeGenerator.cpp ShapeRenderer.cpp Camera.cpp Object.cpp UI.cpp FontRendering.cpp Image.cpp
CYGAME_SRC_FILES := $(patsubst %,src/%,$(CYGAME_SRC_FILES))

INCLUDES := -I./include/ -I/usr/local/include/freetype2 `sdl2-config --cflags --libs`
LINKER_FLAGS := -lfreetype -L/lib/x86_64-linux-gnu/libSDL2-2.0.so.0 -L/usr/local/lib/libfreetype.so.6

build: build/demo build/demo_opengl build/demo_shader

run: build/demo_opengl
	./build/demo_opengl

run2d: build/demo
	./build/demo

run_shader: build/demo_shader
	./build/demo_shader

run_graphs: build/demo_graphs
	./build/demo_graphs

build/demo: demo.cpp include/cygame.h $(CYGAME_SRC_FILES)
	mkdir -p build
	g++ demo.cpp $(CYGAME_SRC_FILES) glad/glad.c $(INCLUDES) $(LINKER_FLAGS) -o build/demo

build/demo_opengl: demo_opengl.cpp include/cygame.h $(CYGAME_SRC_FILES)
	mkdir -p build
	g++ demo_opengl.cpp $(CYGAME_SRC_FILES) glad/glad.c $(INCLUDES) $(LINKER_FLAGS) -o build/demo_opengl

build/demo_graphs: demo_graphs.cpp include/cygame.h $(CYGAME_SRC_FILES)
	mkdir -p build
	g++ demo_graphs.cpp $(CYGAME_SRC_FILES) glad/glad.c $(INCLUDES) $(LINKER_FLAGS) -o build/demo_graphs

build/demo_shader: demo_shader.cpp include/cygame.h $(CYGAME_SRC_FILES)
	mkdir -p build
	g++ demo_shader.cpp $(CYGAME_SRC_FILES) glad/glad.c $(INCLUDES) $(LINKER_FLAGS) -o build/demo_shader

clean:
	rm -f build/demo
	rm -f build/demo_opengl
	rm -f build/demo_shader
