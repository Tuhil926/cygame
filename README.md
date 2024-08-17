# Cygame

- This was my attempt at making a library kind of like pygame, but in c++. This uses sdl2, just like pygame, so a lot of it was pretty straightforward. My main goal was to create easy helper functions and macros around the commonly used stuff in sdl, like the initialisation, input events, or drawing lines, rectangles and circles.
- I'm also working on creating some useful ui components, and so far I've made buttons, text inputs, sliders and text elements.
- This is just meant to be a useful starting point to build your game upon, not a game engine. So I haven't make the most customisable buttons or text inputs or sliders, so it's up to you to change them to fit your game. The main advantage is that you don't have to build this from scratch, or search online for some implementation that may or may not work/fit your preference, or have to install some sort of ui library and learn how to use all it's extensive features.
- Now I know probably no one except me will use this, so I mainly made this for myself. However, if there's something you are looking for that you want added to this, feel free to tell me by creating an issue, and I might add it. Or, you can add it and create a pull request, and if I like it, I'll accept it (I recommend making an issue first).

# Setup

- I tried my best to find apt packages for these libraries, but only sdl2 and sdl2_image seem to have them. So, you'll have to install sdl2_gfx and sdl2_ttf by compiling th source code.
- `sudo apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev`
- download sdl2_gfx from sourceforge(https://sourceforge.net/projects/sdl2gfx/), extract it and cd to the library and run:
  `./autogen.sh`
  `./configure`
  `make`
  `sudo make install`
- and for vscode you might need to add /usr/local/SDL2 to your include path. or just use the headers I provided in the cygame directory.
- download sdl2_ttf from the github page(https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.tar.gz), extract it and cd to it and run these:
  `./configure`
  `make`
  `sudo make install`
- and add /usr/local/lib/SDL2 to path(or just use the headers I provided.), even though it disappeared later for some reason.. idk. It works on my machine now

# To run:

- g++ main.cpp cygame.cpp -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf
