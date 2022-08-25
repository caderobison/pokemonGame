#ifndef CHARACTER_BEHAVIOUR_H
#define CHARACTER_BEHAVIOUR_H
#include "path_finding.h"

extern const int cost_key[13][7];

int init_char(map*, map_cell*, character_t, direction_t); //initializes the character with the specified direction and type
int place_all_chars(map*, int); //puts specified amount of random characters on the map
int move_char(map*, character*); //moves the character based on it's properties
int calculate_new_paths(map*); //calculates new paths for all of the character types except player
int init_all_chars(map*);
int reset_char_times(map*);
// int determine_level(world*);


#endif