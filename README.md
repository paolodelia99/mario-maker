# Mario Maker

Trying to replicate super mario maker with c++ and raylib.

![img1](assets/readme/readme-mario-maker-preview.png)

# Table of contents

- [Quick Start](#quick-start)
- [Creating your own map](#creating-your-own-map)
- [Todos](#todos)

# Quick start 

In order to compile and build the game, the project is provided with a `CMakeList.txt` file, so 
you just need to run:
    
    mkdir build && cd build
    cmake .. 

Additional args could be:

- `-G <Type_of_generator>`

- `-DTMXLITE_INCLUDE_DIR=<your_path_to_txmlib_include>`, the path to the txmlite library, that I've used to parse
the tmx files which contains the structure of the map. 
  
If you're using **CLion** I've included the `./idea` folder, which can help you setting up and build the project.

# Libraries used

- [Raylib](https://github.com/raysan5/raylib) -  used as a framework to build the game
- [Gtest](https://github.com/google/googletest) - used for testing (is install when running the `cmake`)
- [tmxlite](https://github.com/fallahn/tmxlite) - for parsing the txm file used for the map
- [rededev/ECS](https://github.com/redxdev/ECS) - an header only entity component system library (already included)

Since `rededev/ECS` is already included and `Google test` is installed with cmake,
the only two libraries you need to install are Raylib and tmxlite.

## Installing txmlite

### Windows with MinGW

Download git repo:

    git clone https://github.com/fallahn/tmxlite.git
    
Then execute the following command in order to build the library:

    cd tmxlite && cd tmxlite && mkdir build && cd build
    
    # in the build folder run:
    
    cmake .. -G "MinGW Makefiles" 
    
After that, run as an admin the following command:

    mingw32-make clean && mingw32-make install

And now you should have installed the library, just make sure that you 
set the `-DTMXLITE_INCLUDE_DIR` variable when running the CMake file.

### Linux


After you download the repo, using the command listed above run

    cd tmxlite && mkdir build && cd build
    cmake ..
    sudo make install
    
After that you should be done.

# Creating your own map

The map of the game are created using [Tiled](https://www.mapeditor.org/), which is a 2d open source Map editor. 
The file format used for storing the map is `.tmx`, which is based on xml, is easy to understand and allows multiple tileset to 
be used in any map.
The map is composed by layer, which in this case are:

![map_layers](assets\readme\map_layers.PNG)

- a **Background layer**: the ***tile layer*** which contains all the tiles of the background of the level
- a **Graphics layer**: the ***tile layer*** which contains the majority of the tiles(ground's tiles, bricks' tiles and so on) 
which are part of the levels

![graphics_layer](assets\readme\graphics_layer.PNG)

- a **Enemies layer**: the ***object layer*** which contains all the enemies, represented by their AABB, or more simply their collision
box, and their Type (GOOMBA, KOOPA_TROPA, TARTOSSO, ...)
- a **Object layer**: the ***object layer*** which contains all the objects which are not present in the tileset, but 
they are present in the object tileset. Those objects may be, for example the final flag pole, the various checkpoints, and 
others additional object that can enrich the map.
- a **Pipe Layer**: the ***object layer*** wraps all the pipes which are present in the graphics layer making them collidable 
with the other game's entities. Additionally they can have the bool prop `HAS_PIRANHA_PLAN`, which when is true, it makes periodically
pop out the piranha plant from the pipe

![pipe layer](assets/readme/pipes_layers.PNG)

- a **Ground layer**: the ***object layer*** which wraps all the grounds tiles of the graphics layer, making them collidable entites. 
- a **Bricks layer**: the ***object layer*** which wraps all the brick tiles of the graphics layer, making them collidable entites.
- a **Coin layer**:  the ***object layer*** which wraps all the question mark tiles of the graphics layer, making them collidable entites.
The additional properties that a object, in this layer, can have are the following:
    - **coins**: bool prop which when is true means that the question mark block have a coin in it
    - **super_mario_mushroom**: bool prop which when is true means that the question mark block have the super mario mushroom in it
    - **flame_mushroom**: bool prop which when is true means that the question mark block have the flame mushroom/flower in it
    - **mega_mushroom**: bool prop which when is true means that the question mark block have the mega mushroom in it
    - **one_up_mushroom** bool prop which when is true means that the question mark block have the one up mushroom in it
    
# Todos 

- [ ] State pattern for controlling the player and the enemies (https://gameprogrammingpatterns.com/state.html)
- [ ] FlagSystem (for winning)
- [ ] Enemies to Implement
  - [x] Goomba
  - [x] Koopa Troopa
  - [x] Piranha plant
  - [ ] Browser
  - [ ] Baby Browser
  - [x] Tartosso
  - [ ] Thwomp  
  - [ ] bob-omb 
  - [ ] Hammer Bro
  - [ ] Hammer Heavy Bro 
- [ ] fix animation of brick breaking
- [ ] Sounds and music
- [ ] Bubble after a player die

# Author

Paolo D'Elia

# License

MIT