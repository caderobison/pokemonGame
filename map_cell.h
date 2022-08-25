#ifndef MAP_CELL_H
#define MAP_CELL_H

#define MARKET 1
#define CENTER 2
#define BOULDER 3
#define PATH 4
#define GRASS 5
#define CLEARING 6
#define TREE 7
#define DEFAULT 8
#define PLAYER 9
#define NPC 10

#include <iostream>
#include "character.h"
#include <ncurses.h>

typedef enum terrain_t{
		nothing,
		center,
		market,
		boulder,
		bound, //used as the border of the map (ncurses complained about it being named border)
		tree,
		forest,
		path, //path and entrance are identical
		entrance,

		tall_grass0, //two clearing and grass biomes for simplicity when growing the regions
		tall_grass1,

		clearing0,
		clearing1
} terrain_t;

class map_cell{ //structure containing all info about each cell in the room
private:
    terrain_t terr_i; //terrain_t_t of cell
	int dist_i[7]; //distance from player for each type of npc
    int x_i; //coords
    int y_i;
	heap_node_t *hn_i; //heap node for the path_finding heap
	int terrain_cost_i[7]; //cost of the terrain of that cell for each type of npc
	character *person_i;
public:
    map_cell();
    map_cell(terrain_t, int[7], int[7], character*, int, int);
    ~map_cell();

    int x();
    int y();
    terrain_t terr();
    character* person();
    int cost(character_t);
    int dist(character_t);
    // int[7] cost_arr();
    // int[7] dist_arr();
    heap_node_t *hn();

    bool set_coords(int, int);
    bool set_terr(terrain_t);
    bool set_person(character*);
    bool set_cost(character_t, int);
    bool set_dist(character_t, int);
    bool set_cost_arr(int[7]);
    bool set_dist_arr(int[7]);
    bool set_hn(heap_node_t*);

    bool print_map_cell(bool);


    //friend std::ostream &operator<<(std::ostream &o, const map_cell &c);

};

#endif