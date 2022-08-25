#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "character_behaviour.h"

static int counter;
static int NUM_CHARS;
static const int MOVE_X[8] = {0,1,0,-1,1,1,-1,-1}; //includes diagononals 
static const int MOVE_Y[8] = {1,0,-1,0,1,-1,-1,1};
static const int MOVE_DIR_X[5] = {0,0,-1,1,0}; //only cardinal directions. last if for direction_t none
static const int MOVE_DIR_Y[5] = {1,-1,0,0,0};

//static int teleport_player(map_t*);
static bool place_char(map*, character_t, int,int);
//static int check_next_to_player(map_t *room);

int32_t compare_char_time(const void *key, const void *with){ //compare two characters on the map
    character *char_key = (character*)key;
    character *char_with = (character*)with;
    if (char_key->time() - char_with->time()) return char_key->time() - char_with->time();
    else if(char_with->type() == player) return 1;
    else if (char_key->type() == player) return -1;
    else return 0;
}
int init_all_chars(map *room){
    counter = 0;
    int i;
    NUM_CHARS = room->num_chars();
    // room->all_chars = (character**)malloc((NUM_CHARS + 1) * sizeof(character));
    room->set_char_heap((heap_t *)malloc(sizeof(heap_t)));
    heap_init(room->char_heap(), compare_char_time, NULL);
    place_all_chars(room, NUM_CHARS);
    for (i = 0; i < NUM_CHARS; i++){
        if (room->get_char(i)->type() != stationary /*&& room->get_char(i)->type() != player*/){ //no need to put any stationaries into the heap because they never move
            room->get_char(i)->set_hn(heap_insert(room->char_heap(), room->get_char(i)));
        }
    }
    calculate_new_paths(room);
    return 0;
}
int reset_char_times(map *room){
    int i;
    character *npc;
    while((npc=(character*)heap_remove_min(room->char_heap()))){
        //npc = heap_remove_min(room->char_heap);
        npc->set_time(0);
    }
    for(i = 1; i < room->num_chars(); i++){
        room->get_char(i)->set_def(false);
        if (room->get_char(i)->type() != stationary /*&& room->get_char(i)->type() != player*/){
            room->get_char(i)->set_hn(heap_insert(room->char_heap(), room->get_char(i)));
        }
    }
    return 0;
}


int place_all_chars(map *room, int num){
    int i;
    character_t placer;
    if (room->pc_y() == -1 && room->pc_x() == -1){
        place_pc(room);
        init_char(room, room->cell(room->pc_x(), room->pc_y()), player, none);
    }
    else{
        // room->set_char(room->cell(room->pc_x(), room->pc_y())->person(), 0);
        room->pc_cell()->person()->set_hn(NULL);
        // counter++;
    }
    if (num < 0) return 1; //number can't be less than 0;
    else if (num == 0) return 0;
    else if(num < 2){
        placer = hiker;
        if (rand() % 2) placer = rival; 
        while(!place_char(room, placer, (rand() -1)% (LENGTH - 3) + 1, (rand() -1)% (HEIGHT - 3) + 1)){

        } //these while statements ensure that the characters are placed at 
                                                                                                    //valid locations and none are placed on terrains they cannot traverse
    }
    else {
        while(!place_char(room, hiker, (rand() -1)% (LENGTH - 3) + 1, (rand() -1)% (HEIGHT - 3) + 1)){

        }
        while(!place_char(room, rival, (rand() -1)% (LENGTH - 3) + 1, (rand() -1)% (HEIGHT - 3) + 1)){

        }
        for (i = 0; i < num - 2; i++){
            do {placer = (character_t)(rand() % NUM_CHAR_TYPES);} while(placer == player); //keep picking a type until the type is not a player
            while(!place_char(room, placer, (rand() -1)% (LENGTH - 3) + 1, (rand() -1)% (HEIGHT - 3) + 1)){}
        }
    }
    return 0;
}


int init_char(map* room, map_cell *cell, character_t person_type, direction_t direction){
    cell->set_person(new character(direction, person_type, cell->x(), cell->y()));
    cell->person()->set_def(0);
    cell->person()->set_time(0);
    if (person_type != player){
        room->set_char(cell->person(), counter);
        counter++;
    }
    
    if (person_type != player) cell->person()->gen_poke(room->get_x(), room->get_y());
    return 0;
}



int calculate_new_paths(map *room){
    int i;
    for (i = 0; i < NUM_CHAR_TYPES; i++){
        if((character_t)i != player){
            calculate_all_paths(room, (character_t)i);
        }
    }
    return 0;
}

// static int check_next_to_player(map_t *room){
//     int newX, newY, i;
//     character *enemy;
//     for (i = 0; i < 8; i++){
//         newY = room->pc_y + MOVE_Y[i];
//         newX = room->pc_x + MOVE_X[i];
//         enemy = room->map[newY][newX]->person;
//         if (enemy && (enemy->type == hiker || enemy->type == rival || MOVE_X[i] ^ MOVE_Y[i])){ //when diagonal from player, only count
//                                                                                             //as "next to" when the enemy is a hiker or rival
//             return 1;
//         }
//     }
//     return 0;
// }
//
// static int teleport_player(map_t *room){
//     character *player_char = room->map[room->pc_y][room->pc_x]->person;
//     room->map[room->pc_y][room->pc_x]->person = NULL;
//     do{
//     room->pc_x = (rand() -1)% (LENGTH - 3) + 1;
//     room->pc_y = (rand() -1)% (HEIGHT - 3) + 1;
//     } while(room->map[room->pc_y][room->pc_x]->terrain_cost[player] >= INT_MAX || room->map[room->pc_y][room->pc_x]->terr == market
//      || room->map[room->pc_y][room->pc_x]->terr == center || check_next_to_player(room) || room->map[room->pc_y][room->pc_x]->person);
//      //cannot place player if any of these conditions are true
//      //1. the cell is a market or center
//      //2. the cell's terrain is infinity for the player
//      //3. there is an enemy next to the player
//      //4. there is an npc at the cell
//     room->map[room->pc_y][room->pc_x]->person = player_char;
//     player_char->x = room->pc_x;
//     player_char->y = room->pc_y;
//     return 0;
// }


static bool place_char(map *room, character_t person, int x, int y){
    int i, newY, newX;
    if (room->cell(x,y)->cost(person) >= INT_MAX || person == player || room->cell(x,y)->person()){ //can't place a player or if the cell's terrain cost
                                                                                //is infinite
        return false;
    }
    //these next for loops make it so a char cannot be placed on a cell surrounded by cells with infinite cost
    if (person == hiker || person == rival){ //include diagonals for hikers and rivals, only cardinals for everyone else
        for (i = 0; i < 8; i++){
            newY = y + MOVE_Y[i];
            newX = x + MOVE_X[i];
            if (room->cell(newX, newY)->cost(person) < INT_MAX) {
                break;
            }
            return false;
        }
    }
    else{
        for (i = 0; i < 4; i++){
            newY = y + MOVE_DIR_Y[i];
            newX = x + MOVE_DIR_X[i];
            if (room->cell(newX, newY)->cost(person) < INT_MAX) {
                break;
            }
            return false;
        }
    }
    if (person == wanderer || person == pacer || person == random_walker){ //only wanderers, pacers and random_walkers need directions
        init_char(room, room->cell(x,y), person, (direction_t)(rand() % 4));
    }
    else init_char(room, room->cell(x,y), person, none);

    return true;
}


int move_char(map *room, character *person){
    int canMove = 0;
    direction_t new_dir;
    int i;
    int changeDir = 0; //0 if the person has not changed directions, 1 if they have
    map_cell *next = room->cell(person->x() + MOVE_DIR_X[person->dir()], person->y() + MOVE_DIR_Y[person->dir()]); //go to the cell person wants to move to
    map_cell *test;
    switch(person->type()){
        case stationary: //stationaries don't move
            break;
        case player: //players don't move with this function
            break;
        case hiker:
        case rival:
            for (i = 0; i < 8; i++){
                test = room->cell(person->x() + MOVE_X[i],person->y() + MOVE_Y[i]);
                if (!test->person() && test->dist(person->type()) < INT_MAX && test->cost(person->type()) < INT_MAX){
                    next = test;
                    canMove = 1;
                    break;
                }
            }
            if (!canMove){
                person->add_time(room->cell(person->x(), person->y())->cost(person->type()));
                return 1;
            }
            for (i = 0; i < 8; i++){
                test = room->cell(person->x() + MOVE_X[i], person->y() + MOVE_Y[i]); 
                if((test->dist(person->type()) < next->dist(person->type()) || 
                (test->dist(person->type()) == next->dist(person->type()) && test->cost(person->type()) < next->cost(person->type()) )) 
                 && (test->person() == NULL || test->person()->type() == player)){ //only move to the test cell if the distance to player is shorter than before, OR
                                            //the distances are the same but the next terrain is cheaper. Also be sure there is no npc there
                    next = test;
                }
            }
            break;
        case pacer:
            if (next->cost(person->type()) >= INT_MAX || (next->person() && next->person()->type() != player)){ //if a pacer hits an npc or an infinite block, turn around
                changeDir = 1;
                switch(person->dir()){
                    case north:
                        person->set_dir(south);
                        break;
                    case south:
                        person->set_dir(north);
                        break;
                    case east:
                        person->set_dir(west);
                        break;
                    case west:
                        person->set_dir(east);
                        break;
                    case none:
                    default:
                        break;
                }
            }
            break;
        case random_walker: 
        case wanderer:{ ;
            terrain_t cell_terr = room->cell(person->x(), person->y())->terr();
            if ( (next->person() && next->person()->type() != player) || next->cost(person->type()) >= INT_MAX || 
            (person->type() == wanderer && cell_terr != next->terr())){
                for (i = 0; i < 4; i++){
                    new_dir = (direction_t)(north + i);
                    if (room->cell(person->x() + MOVE_DIR_X[new_dir],person->y() + MOVE_DIR_Y[new_dir])->cost(person->type()) < INT_MAX
                    && !room->cell(person->x() + MOVE_DIR_X[new_dir],person->y() + MOVE_DIR_Y[new_dir])->person() && 
                    (person->type() != wanderer || room->cell(person->x() + MOVE_DIR_X[new_dir],person->y() + MOVE_DIR_Y[new_dir])->terr() == cell_terr)){
                        canMove = 1;
                        break;
                    }
                }
                if (!canMove){
                    person->add_time(room->cell(person->x(), person->y())->cost(person->type()));
                    return 1;
                }
                do{
                    new_dir = (direction_t) (rand() % 4); //choose a random direction that won't lead the npc into a terrain they can't cross
                                                            //or another npc
                }while(new_dir == person->dir() || 
                room->cell(person->x() + MOVE_DIR_X[new_dir],person->y() + MOVE_DIR_Y[new_dir])->cost(person->type()) >= INT_MAX
                || (person->type() == wanderer && room->cell(person->x() + MOVE_DIR_X[new_dir],person->y() + MOVE_DIR_Y[new_dir])->terr() !=cell_terr)
                || (room->cell(person->x() + MOVE_DIR_X[new_dir],person->y() + MOVE_DIR_Y[new_dir])->person()));
                changeDir = 1;
                person->set_dir(new_dir);
            }
            break;
        }
        default: {
            break;
        }
    }
    if (changeDir) {
        next = room->cell(person->x() + MOVE_DIR_X[person->dir()],person->y() + MOVE_DIR_Y[person->dir()]); 
        if ((next->person() && next->person()->type() != player) || next->cost(person->type()) >= INT_MAX) {
            person->add_time(room->cell(person->x() ,person->y() )->cost(person->type()));
        }
    }
    if (!((next->person() && next->person()->type() != player) || next->cost(person->type()) >= INT_MAX)){
        person->add_time(next->cost(person->type()));
    }
    if (next->person() && next->person()->type() == player){
        return 2;
    }
    if (next->x() != room->cell(person->x(),person->y())->x() || next->y() != room->cell(person->x(),person->y())->y()){ //if the npc doesn't move, no need to do all this
        next->set_person(person);
        room->cell(person->x(),person->y())->set_person(NULL);

        person->set_x(next->x());
        person->set_y(next->y());
    }
    return 0;
}



