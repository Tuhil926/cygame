all: build


build:
	g++ main.cpp cygame.cpp `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_gfx -lSDL2_ttf
