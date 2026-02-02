all: build

CYGAME_SRC_FILES := cygame.cpp Shape.cpp ShapeGenerator.cpp ShapeRenderer.cpp Camera.cpp Object.cpp UI.cpp FontRendering.cpp Image.cpp
CYGAME_SRC_FILES := $(patsubst %,src/%,$(CYGAME_SRC_FILES))
CYGAME_OBJ_FILES := $(patsubst src/%.cpp,build/%.o,$(CYGAME_SRC_FILES))

INCLUDES := -I./include/ -I/usr/local/include/freetype2 `sdl2-config --cflags`
LINKER_FLAGS := `sdl2-config --libs` -lfreetype -lGL

build: build/demo build/demo_opengl build/demo_shader build/demo_graphs

run: build/demo_opengl
	./build/demo_opengl

run2d: build/demo
	./build/demo

run_shader: build/demo_shader
	./build/demo_shader

run_graphs: build/demo_graphs
	./build/demo_graphs

build/%.o: src/%.cpp
	mkdir -p build
	g++ $(INCLUDES) -c $< -o $@

build/glad.o: glad/glad.c
	mkdir -p build
	g++ $(INCLUDES) -c glad/glad.c -o build/glad.o

build/libcygame.a: $(CYGAME_OBJ_FILES) build/glad.o
	ar rcs $@ $^

build/demo: demo.cpp include/cygame.h build/libcygame.a
	g++ demo.cpp build/libcygame.a $(INCLUDES) $(LINKER_FLAGS) -o build/demo

build/demo_opengl: demo_opengl.cpp include/cygame.h build/libcygame.a
	g++ demo_opengl.cpp build/libcygame.a $(INCLUDES) $(LINKER_FLAGS) -o build/demo_opengl

build/demo_graphs: demo_graphs.cpp include/cygame.h build/libcygame.a
	g++ demo_graphs.cpp build/libcygame.a $(INCLUDES) $(LINKER_FLAGS) -o build/demo_graphs

build/demo_shader: demo_shader.cpp include/cygame.h build/libcygame.a
	g++ demo_shader.cpp build/libcygame.a $(INCLUDES) $(LINKER_FLAGS) -o build/demo_shader

clean:
	rm -f build/demo
	rm -f build/demo_opengl
	rm -f build/demo_shader
	rm -f build/demo_graphs
	rm -f build/*.o
	rm -f build/libcygame.a
