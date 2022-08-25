#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include "world.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int place_pc(map*); //place the player in a random spot on a path
int calculate_all_paths(map*, character_t); //Use dijkstras to calculate shortest path to player from all cells


#endif