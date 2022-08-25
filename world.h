#ifndef WORLD_H
#define WORLD_H

#include "map.h"
#include <iostream>

#define W_HEIGHT 399
#define W_LENGTH 399

class world{
private:
    map* maps[W_HEIGHT][W_LENGTH];
    int X; //current x map-position in the world
    int Y; //current y

    bool build_room();
    bool add_set_entrances();
public:
    world();
    ~world();
    
    map* cmap(int x, int y);
    map* mapn();
    int x();
    int y();

    bool set_map(map*);
    bool set_coord(int x, int y);
    bool move_map(direction_t);

};

#endif