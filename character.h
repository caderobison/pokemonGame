#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "heap.h"
#include "parsing.h"
#include <vector>

#define REVIVE 4
#define POTION 5
#define BALL 6
#define OFFSET 4

#define FIGHT 0
#define BAG 1
#define RUN 2
#define CHANGE 3

#define MOVE1 7
#define MOVE2 8

extern pokemon *all_pokemon;
extern poke_move *all_moves;
extern pokemon_move *all_pokemon_moves;
extern stat *all_stat;
extern pokemon_species *all_species;
extern experience *all_exp;
extern type_name *all_types;
extern pokemon_type* all_pokemon_types;

typedef enum character_t{
    hiker,
    rival,
    player,
    pacer,
    wanderer,
    stationary,
    random_walker
} character_t;

typedef enum direction_t{
    north,
    south,
    west,
    east,
    none,
    south_e,
    north_e,
    north_w,
    south_w
}direction_t;

int damage(specific_poke&, poke_move&);
int determine_level(int, int);

class character{
private:
    character_t type_i;
    direction_t dir_i;
    int x_i; //i stands for internal
    int y_i;
    int time_i;
    heap_node_t *hn_i;
    bool defeated_i;
    std::vector<specific_poke*> *roster;
    // int poke_counter;
    int inventory[3];
    specific_poke *active_poke;
public:
    character();
    character(direction_t, character_t, int, int);
    ~character();

    int x();
    int y();
    direction_t dir();
    character_t type();
    int time();
    bool def();
    heap_node_t *hn();
    specific_poke& get_poke(int);
    int get_num_poke();
    // specific_poke** get_all_poke();

    bool set_x(int);
    bool set_y(int);
    bool set_dir(direction_t);
    bool set_type(character_t);
    bool set_time(int);
    bool add_time(int);
    bool set_def(bool);
    bool set_hn(heap_node_t*);
    bool add_poke(specific_poke&);
    bool delete_poke(int);
    bool use_item(int i, bool, specific_poke*);
    int get_item(int);
    int* get_inventory();
    specific_poke* get_active();
    bool set_active(int);
    bool set_active(specific_poke&);
    // bool print();
    int choose_move();
    bool commit_move(int, specific_poke&, bool, int);
    bool gen_poke(int, int);



    // character &operator=(const  character &c);
    // friend std::ostream &operator<<(std::ostream &o, const character &c);

};

#endif