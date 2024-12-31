all: build

CYGAME_SRC_FILES := cygame.cpp Shape.cpp ShapeGenerator.cpp ShapeRenderer.cpp Camera.cpp Object.cpp UI.cpp

build: demo demo_opengl

run: demo_opengl
	./demo_opengl

run2d: demo
	./demo

demo: main.cpp cygame.h $(CYGAME_SRC_FILES)
	g++ main.cpp $(CYGAME_SRC_FILES) glad/glad.c -I./include/ `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf -o demo

demo_opengl: main_opengl.cpp cygame.h $(CYGAME_SRC_FILES)
	g++ main_opengl.cpp $(CYGAME_SRC_FILES) glad/glad.c -I./include/ `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf -o demo_opengl
