#include "map_cell.h"

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

map_cell::map_cell(){
    int i;
    terr_i = nothing;
    x_i = -1;
    y_i = -1;
    hn_i = NULL;
    person_i = NULL;
    for (i = 0; i < 7; i++){
        terrain_cost_i[i] = -1;
        dist_i[i] = -1;
    }
}
map_cell::map_cell(terrain_t terr, int dist[7], int terrain_cost[7], character *person, int x, int y){
    int i;
    terr_i = terr;
    person_i = person;
    x_i = x;
    y_i = y;
    for (i = 0; i < 7; i++){
      if(dist){
        dist_i[i] = dist[i];
      }
      if(terrain_cost){
        terrain_cost_i[i] = terrain_cost[i];
      }
    }
}
map_cell::~map_cell(){
    //if (person_i) delete person_i;
    //if (hn_i) free(hn_i);
}

int map_cell::x(){
    return x_i;
}
int map_cell::y(){ 
    return y_i;
}
terrain_t map_cell::terr(){
    return terr_i;
}
character* map_cell::person(){
    return person_i;
}
int map_cell::cost(character_t type){
    if ((int)type >= 7 || (int)type < 0 || !terrain_cost_i) return -1;
    else return terrain_cost_i[type];
}
int map_cell::dist(character_t type){
    if ((int)type >= 7 || (int)type < 0 || !dist_i) return -1;
    else return dist_i[type];
}
// int[7] cost_arr(){
//     return terrain_cost;
// }
// int[7] dist_arr(){
//     return dist;
// }
heap_node_t *map_cell::hn(){
    return hn_i;
}

bool map_cell::set_coords(int x, int y){
    x_i = x;
    y_i = y;
    return true;
}
bool map_cell::set_terr(terrain_t terr){
    terr_i = terr;
    return true;
}
bool map_cell::set_person(character* person){
    person_i = person;
    return true;
}
bool map_cell::set_cost(character_t type, int cost){
    if ((int)type >= 7 || (int)type < 0) return false;
    terrain_cost_i[type] = cost;
    return true;
}
bool map_cell::set_dist(character_t type, int dist){
    if ((int)type >= 7 || (int)type < 0) return false;
    dist_i[type] = dist;
    return true;
}
bool map_cell::set_cost_arr(int cost[7]){
    for (int i = 0; i < 7; i++){
        terrain_cost_i[i] = cost[i];
    }
    return true;
}
bool map_cell::set_dist_arr(int dist[7]){
    for (int i = 0; i < 7; i++){
        dist_i[i] = dist[i];
    }
    return true;
}
bool map_cell::set_hn(heap_node_t* hn){
    hn_i = hn;
    return true;
}
//assume that curses has already been initialized
bool map_cell::print_map_cell(bool colors){
  if(person_i){
        // printf("\033[0;31m");
        if (person_i->type() != player){
          if (colors){
            attron(COLOR_PAIR(NPC));
          }
          else attron(COLOR_PAIR(CENTER));
        }
        switch(person_i->type()){
          case player:{ ;
            //printf("\033[0;34m");
            int c = MARKET;
            if (colors) c = PLAYER;
            attron(COLOR_PAIR(c));
            printw("@");
            attroff(COLOR_PAIR(c));
            break;
          }
          case hiker:{
            printw("h");
            break;
          }
          case rival:{
            printw("r");
            break;
          }
          case pacer:
            {printw("p");
            break;}
          case wanderer:
            {printw("w");
            break;}
          case stationary:
            {printw("s");
            break;}
          case random_walker:
            {printw("n");
            break;}
          default:
            {printw("7");
            break;}
        }
        if (person_i->type() != player){
          if (colors){
            attroff(COLOR_PAIR(NPC));
          }
          else attroff(COLOR_PAIR(CENTER));
        }
        return true;
      }
      terrain_t type = terr();
      switch(type){
      case nothing: //never in a finished map, used for debugging
	      attron(COLOR_PAIR(DEFAULT));
        printw(" ");
        attroff(COLOR_PAIR(DEFAULT));
	      break;
      case boulder:
      case bound:
        attron(COLOR_PAIR(BOULDER));
        //printf("\033[0;37m"); //print white
        printw("%%");
        attroff(COLOR_PAIR(BOULDER));
	      break;
      case entrance: //same as path
      case path:
        attron(COLOR_PAIR(PATH));
        //printf("\033[0;33m"); // yellow
        printw("#");
        attroff(COLOR_PAIR(PATH));
        break;
      case market:
        attron(COLOR_PAIR(MARKET));
        //printw("\033[0;34m"); // blue
        printw("M");
        attroff(COLOR_PAIR(MARKET));
        break;
      case center:
        attron(COLOR_PAIR(CENTER));
        //printw("\033[0;31m"); //red
        printw("C");
        attroff(COLOR_PAIR(CENTER));
        break;
      case tall_grass0:
      case tall_grass1:
        attron(COLOR_PAIR(GRASS));
        //printw("\033[0;32m"); //green
        printw(",");
        attroff(COLOR_PAIR(GRASS));
        break;
      case clearing0:
      case clearing1:
        attron(COLOR_PAIR(CLEARING));
        //printw("\033[0;35m"); //purple
        printw(".");
        attroff(COLOR_PAIR(CLEARING));
        break;
      case forest:
        attron(COLOR_PAIR(TREE));
        //printw("\033[0;36m"); //cyan
        printw("\"");
        attroff(COLOR_PAIR(TREE));
        break;
      case tree:
        attron(COLOR_PAIR(TREE));
        //printw("\033[0;36m"); //cyan
        printw("^");
        attroff(COLOR_PAIR(TREE));
        break;
      default: //should NEVER happen, used for debugging
        printw("b");
      	break;
      }
    //printw("\n"); 
    return true;
}

