all: build

CYGAME_SRC_FILES := cygame.cpp Shape.cpp ShapeGenerator.cpp ShapeRenderer.cpp Camera.cpp Object.cpp UI.cpp FontRendering.cpp

build: build/demo build/demo_opengl

run: build/demo_opengl
	./build/demo_opengl

run2d: build/demo
	./build/demo

build/demo: demo.cpp cygame.h $(CYGAME_SRC_FILES)
	mkdir -p build
	g++ demo.cpp $(CYGAME_SRC_FILES) glad/glad.c -I./include/ -I/usr/local/include/freetype2 `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lfreetype -L/lib/x86_64-linux-gnu/libSDL2-2.0.so.0 -L/usr/local/lib/libfreetype.so.6 -o build/demo

build/demo_opengl: demo_opengl.cpp cygame.h $(CYGAME_SRC_FILES)
	mkdir -p build
	g++ demo_opengl.cpp $(CYGAME_SRC_FILES) glad/glad.c -I./include/ -I/usr/local/include/freetype2 `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lfreetype -L/lib/x86_64-linux-gnu/libSDL2-2.0.so.0 -L/usr/local/lib/libfreetype.so.6 -o build/demo_opengl

clean:
	rm -f build/demo
	rm -f build/demo_opengl
