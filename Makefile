all: build


build: demo demo_opengl

run: demo_opengl
	./demo_opengl

demo: main.cpp cygame.cpp cygame.h
	g++ main.cpp cygame.cpp glad/glad.c -I./include/ `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf -o demo

demo_opengl: main_opengl.cpp cygame.cpp cygame.h
	g++ main_opengl.cpp cygame.cpp glad/glad.c -I./include/ `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf -o demo_opengl
