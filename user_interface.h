#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include "character_behaviour.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "parsing.h"

extern const int cost_key[13][7];

extern int32_t compare_char_time(const void *key, const void *with);

int move_and_print_chars(world*);
int perform_instr(int, world*);
int move_player(world*, direction_t);
int show_trainers(map*);
int fight_npc(world*, character*);
int enter_building(map*);


#endif