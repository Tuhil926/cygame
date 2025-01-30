all: build

CYGAME_SRC_FILES := cygame.cpp Shape.cpp ShapeGenerator.cpp ShapeRenderer.cpp Camera.cpp Object.cpp UI.cpp FontRendering.cpp

build: demo demo_opengl

run: demo_opengl
	./demo_opengl

run2d: demo
	./demo

demo: demo.cpp cygame.h $(CYGAME_SRC_FILES)
	g++ demo.cpp $(CYGAME_SRC_FILES) glad/glad.c -I./include/ -I/usr/local/include/freetype2 `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf -lfreetype -o demo

demo_opengl: demo_opengl.cpp cygame.h $(CYGAME_SRC_FILES)
	g++ demo_opengl.cpp $(CYGAME_SRC_FILES) glad/glad.c -I./include/ -I/usr/local/include/freetype2 `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf -lfreetype -o demo_opengl
