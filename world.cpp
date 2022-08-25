#include "world.h"

world::world(){
  X = W_LENGTH/2; //Make the first map at 0,0
  Y = W_HEIGHT/2;
  maps[Y][X] = new map();
  maps[Y][X]->set_coords(X,Y);

  //map_init_rand(world->maps[world->Y][world->X]); //guarantees shops will be generated 
}
world::~world(){
  // delete this->mapn()->pc_cell()->person();
    for(int i = 0; i < W_HEIGHT; i++){
        for (int j = 0; j < W_LENGTH; j++){
            if(maps[i][j]) {
                delete maps[i][j];
            }
        }
    }
}

map* world::cmap(int x, int y){
    if(x<0 || x >= W_LENGTH || y < 0 || y >=W_HEIGHT) return NULL;
    return maps[y][x];
}

map* world::mapn(){
    return maps[Y][X];
}
int world::x(){
    return X;
}
int world::y(){
    return Y;
}

bool world::set_map(map* room){
    if(maps[Y][X]) delete maps[Y][X];
    maps[Y][X] = room;
    return true;
}
bool world::set_coord(int x, int y){
    if(x<0 || x >= W_LENGTH || y < 0 || y >=W_HEIGHT) return false;
    X = x;
    Y = y;
    return true;
}

bool world::move_map(direction_t command){
  switch(command){
    case south:
      if (Y+1 < W_HEIGHT){ Y+=1;}
      break;
    case north:
      if (Y-1 >= 0){ Y-=1;}
      break;
    case east:
      if (X+1 < W_LENGTH){ X+=1;}
      break;
    case west:
      if (X-1 >= 0){ X-=1;}
      break;
    default:
	    printw("Command not found\n");
    }

  if (maps[Y][X] == NULL){
    build_room(); //build a room at current x and y
  }
  return true;
}

bool world::build_room(){ //builds room for CURRENT x and y
  //int i;
  map *room = new map(X, Y);  //room made as temp pointer
  maps[Y][X] = room;

  add_set_entrances(); //add all entrances set by other maps, and random entrances
  bool shops = false;
  if(room->cell(LENGTH/2-2, HEIGHT/2 - 1)->terr() == center) shops = true; //Only time there is a center at this location is
                                                                //when there are shops on the map
  room->add_all_paths(shops);
  room->add_terrain_cost();
  return true;
}

bool world::add_set_entrances(){ //add the entrances set by the bordering maps
  int i;
  
  int nextBorders[4] = {south,north,east,west};  //Index of what directed each map's entrance will be on the new map 
                                    //(0 = n, 1 = s, 2=w, 3 = e )
                                    //ex. if current has a north (0) entrance at x, and I go north, the map above current
                                    //will have a south (1) entrance equal to current's north. so nextBorders[0] = 1
  int x = X;
  int y = Y;
  int borderingX[4] = {x,x,x-1,x+1}; //x coords of all neighboring maps
  int borderingY[4] = {y-1,y+1,y,y}; //y coords

  map *current = maps[y][x];
  map *next;

  for (i = 0; i < 4; i++){
    if(borderingX[i] < 0 || borderingX[i] >= W_LENGTH || borderingY[i] < 0 || borderingY[i] >= W_HEIGHT) { //if on the
      //world border, don't put an entrance at that wall (entrance = -1)
      current->add_one_entrance((direction_t)i, -1);
      continue;
    }
    next = maps[borderingY[i]][borderingX[i]];
    if(next == NULL) { //next hasn't been loaded, choos random coordinate
      current->add_one_entrance((direction_t)i, rand());
    }
    else if(next->entrance((direction_t)nextBorders[i])>0){ //entrance of next lines up with map
      maps[Y][X]->add_one_entrance((direction_t)i, next->entrance((direction_t)nextBorders[i]));
    }
  }
  return true;
}
