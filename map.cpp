#include "map.h"
#include <climits>


const int cost_key[13][7] = {{7,7,7,7,7,7,7}, //nothing (should never happen)
                    {INT_MAX, INT_MAX, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}, //center 
                    {INT_MAX, INT_MAX, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}, //market 
                    {15, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX}, //boulder (mountains) 
                    {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX}, //bound 
                    {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX}, //tree 
                    {15, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX}, //forest 
                    {10,10,10, 10, 10, 10, 10}, //path 
                    {INT_MAX,INT_MAX,10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}, //entrance 
                    {15,20,20, 20, 20, 20, 20}, //tall grass_0 
                    {15,20,20, 20, 20, 20, 20}, //tall grass_1 
                    {10,10,10, 10, 10, 10, 10}, //clearing_0 
                    {10,10,10, 10, 10, 10, 10}}; //clearing_1 

static int counter = 0;

#define ENTRANCE 8

static int absoluteValue(int a){ //find the absolute value of a, assume 0 is positive
  if(a < 0){
    return -1 * a;
  }
  return a;
}

int sign(int a){ //return the sign (+ or -) of a
  if(a >=0 ) return 1;
  else return -1;
}

float decide_shops(int d){
  if(d <= 200){
    return (-45.0*d / 200) + 50;
  }
  return 5;
}



map::map(){
  x = -1;
  y = -1;
  pc_x_i = -1;
  pc_y_i = -1;
  init_map();
  addBorders();
  for (int i = 0; i < 4; i++){
    add_one_entrance((direction_t)i, rand());
  }
  addShops();
  growRegion(true);
  add_all_paths(true);
  addDecor();
  add_terrain_cost();
}

map::map(int x, int y){
  this->x = x;
  this->y = y;
  pc_x_i = -1;
  pc_y_i = -1;
  init_map();
  addBorders();

  double z = (double)rand()/(RAND_MAX/100);
  int d = absoluteValue(x + y - 398);
  bool shops = false;
  if (z <= decide_shops(d)){
    addShops();
    shops = true;
  }

  growRegion(shops);
  addDecor();
  add_terrain_cost();
}


map::~map(){
  int i,j;
  
  // if (all_chars_i) delete[] all_chars_i; //need to go one by one?
  remove_char_heap();
  for(i = 0; i < num_chars_i; i++){
	  delete all_chars_i[i];
  }

  if (pc_x_i != -1 && pc_y_i != -1) delete pc_cell()->person();
  delete[] all_chars_i;
  for (i = 0; i < HEIGHT; i++){
    for (j = 0; j < LENGTH; j++){
      if(cells_i[i][j] != NULL){
        //if (cells_i[i][j]->person() != NULL)
        delete cells_i[i][j];
      }
    }
  }
}
map_cell* map::cell(int x, int y){
    if (x >= LENGTH || x < 0 || y >= HEIGHT || y < 0) return NULL;
    return cells_i[y][x];
}

map_cell* map::pc_cell(){
  return cells_i[pc_y_i][pc_x_i];
}
int map::entrance(direction_t index){
    if ((int)index >= 4 || (int)index < 0) return -1;
    return entrances_i[index];
}
//int[4] entrance_arr(){}
int map::path_x(int index){
    if (index >= num_path_i || index < 0) return -1;
    return paths_x_i[index];
}
int map::path_y(int index){
    if (index >= num_path_i || index < 0) return -1;
    return paths_y_i[index];
}
int map::num_path(){
    return num_path_i;
}
int map::num_chars(){
  return num_chars_i;
}
int map::pc_x(){
    return pc_x_i;
}
int map::pc_y(){
    return pc_y_i;
}
character* map::get_char(int index){
    if (index > num_chars_i || index < 0) return NULL;
    return all_chars_i[index];
}

character** map::all_chars(){
  // ret = (character**)malloc((num_chars_i + 1)*sizeof(character));
  // for(int i = 0; i < num_chars_i; i++){
  //   ret[i] = all_chars_i[i];
  // }
  // return ret;
  return all_chars_i;
}
heap_t* map::char_heap(){
  return char_heap_i;
}

int map::get_x(){return x;}
int map::get_y(){return y;}
bool map::set_coords(int x, int y){
  if (this->x != -1 || this->y != -1) return false;
  this->x = x;
  this->y = y;
  return true;
}

bool map::set_char_heap(heap_t *h){
  char_heap_i = h;
  return true;
}

bool map::remove_char_heap(){
  if (!char_heap_i) return false;
  heap_delete(char_heap_i);
  free(char_heap_i);
  char_heap_i = NULL;
  return true;
}

bool map::add_path(int x, int y){
    if (x >= LENGTH || x < 0 || y >= HEIGHT || y < 0) return false;
    paths_x_i[num_path_i] = x;
    paths_y_i[num_path_i] = y;
    num_path_i++;
    return true;
}
bool map::set_pc(int x, int y){
    if (x >= LENGTH || x < -1 || y >= HEIGHT || y < -1) return false;
    pc_x_i = x;
    pc_y_i = y;
    return true;
}


bool map::set_num_chars(int num_chars){
  if(all_chars_i) return false;
  num_chars_i = num_chars;
  //all_chars_i = (character**)malloc((num_chars_i + 1)*sizeof(character));
  all_chars_i = new character*[num_chars_i];
  return true;
}

bool map::add_char(character* person){ //DO NOT USE, IS BROKEN
    if(!all_chars_i) all_chars_i = (character**)malloc((num_chars_i + 1)*sizeof(character));
    if(!person) return false;
    all_chars_i[counter] = person;
    counter++;
    return true;
}

bool map::set_char(character* person, int index){
  if (!all_chars_i) return false;
  // if(!all_chars_i) all_chars_i = (character**)malloc((num_chars_i + 1)*sizeof(character));
  if(num_chars_i < index) return false;
  all_chars_i[index] = person;
  return true;
}


bool map::add_terrain_cost(){
  int i,j,k;
  for (i = 0; i < HEIGHT; i++){
    for (j = 0; j < LENGTH; j++){
      for (k = 0; k < 7; k++){
        cells_i[i][j]->set_cost( (character_t)k, cost_key[cells_i[i][j]->terr()][k]);
      }
    }
  }
  return true;
}

bool map::addBorders(){
  int i;
  for (i = 0; i < HEIGHT; i++){
    cells_i[i][0]->set_terr(bound);
    cells_i[i][LENGTH-1]->set_terr(bound);
  }
  for(i = 0; i < LENGTH; i++){
    cells_i[0][i]->set_terr(bound);
    cells_i[HEIGHT-1][i]->set_terr(bound);
  }
  return true;
}

bool map::init_map(){
  int i,j;
  num_chars_i = 0;
  num_path_i = 0;
  all_chars_i = NULL;
  char_heap_i = NULL;
  for(i = 0; i < HEIGHT; i++){
    for(j = 0; j < LENGTH; j++){
      cells_i[i][j] = new map_cell(nothing, NULL, NULL, NULL, j, i);
    }
  }
  for (i = 0; i < 4; i++){
    entrances_i[i] = 0;
  }
  for(i = 0; i < 2*(HEIGHT + LENGTH); i++){
    paths_x_i[i] = -1;
    paths_y_i[i] = -1;
  }
  pc_x_i = -1;
  pc_y_i = -1;
  return true;
}

bool map::add_one_entrance(direction_t dir, int coord){
  switch (dir){
  case north: 
    coord = (coord -1)% (LENGTH - 3) + 1; //guarantees a number between 1 and LENGTH-1 inclusive while not changing
                                          //with coord if coord < LENGTH - 1
    cells_i[0][coord]->set_terr((terrain_t)ENTRANCE); // place entrance at designated spot
    entrances_i[north] = coord; //put relevant coordinate in cooresponding entreances listing. 0 is north
    break;
  case south:
    coord = (coord-1) % (LENGTH - 3) + 1;
    cells_i[HEIGHT-1][coord]->set_terr((terrain_t)ENTRANCE);
    entrances_i[south] = coord; //south
    break;

  case west:
    coord = (coord - 1) % (HEIGHT - 3) + 1;
    cells_i[coord][0]->set_terr((terrain_t)ENTRANCE);
    entrances_i[west] = coord; //west
    break;

  case east:
    coord = (coord - 1) % (HEIGHT - 3) + 1;
    cells_i[coord][LENGTH-1]->set_terr((terrain_t)ENTRANCE);
    entrances_i[east] = coord; //east
    break;
  default: return false;
  }
  return true;
}

bool map::addShops(){ //leaves path around each shop
  int i,j;
  int aveH = HEIGHT / 2;
  int aveL = LENGTH / 2;
  for(i = aveH -1; i <= aveH; i++){ //left side of block is a 2x2 center
    for(j = aveL-2; j <= aveL-1; j++){
      cells_i[i][j]->set_terr(center);
      //remove_path(room, j,i);
     }
  } 

  for(i = aveH-1; i <= aveH; i++){ //right side of block is 2x2 market
    for(j = aveL+1; j <= aveL+2; j++){
      cells_i[i][j]->set_terr(market);
      //remove_path(room, j,i);
    }
  }
  for(i = aveH-2; i <= aveH+1; i++){ //create a 7x4 block of paths in the middle of the map
    for(j = aveL-3; j <= aveL +3; j++){
      if (cells_i[i][j]->terr() != center && cells_i[i][j]->terr() != market){
        cells_i[i][j]->set_terr(path);
        add_path(j,i);
      }
    }
  }
  return true;
}


bool map::growRegion(bool shop_bool){
  #define MOVE_NUM  20
  int move_x[MOVE_NUM] = {0,1,1,1,0,-1,-1,-1, 2,2,2,-2,-2,-2, 3,3,3,-3,-3,-3}; //every iteration, grow region in 3x7 square
                                                                              //can go 3 blocks in lateral directions, 1 block in vertical
  int move_y[MOVE_NUM] = {1,1,0,-1,-1,-1,0,1, 1,0,-1,1,0,-1,1,0,-1,1,0,-1};
  int counters[5] = {1,1,1,1,1}; //count number of each reigion
  int i, j, k;
  terrain_t terrains[5] = {tall_grass0, tall_grass1, clearing0, clearing1, boulder}; //all types of reigions. regions[4] can also be tree
  //int seed = time(NULL);
  //srand(seed);
  int choose = rand() % 2;
  if(choose == 0){
    terrains[4] = forest; //50/50 chance of last region being tree
  }
  int valid_y[5][HEIGHT*LENGTH]; //lists all y coordinates that have terrain type i. second number is an index. 
  int valid_x[5][LENGTH*HEIGHT]; //lists all x coordinates that have terrain type i
  int goAhead;
  for(i = 0; i < 5; i++){ //choosing starting seeds
    goAhead = 0;
    while(goAhead == 0){ //while we don't have a valid starting seed
      valid_x[i][0] = rand() % LENGTH; //choose random x and y coords
      valid_y[i][0] = rand() % HEIGHT;
      if(cells_i[valid_y[i][0]][valid_x[i][0]]->terr()== nothing){ //only make that the starting seed of terrain i if nothing else is there
       cells_i[valid_y[i][0]][valid_x[i][0]]->set_terr(terrains[i]); // set seed as terrain i
       goAhead = 1; //continue to next i;
      }
    }
  }

  int x, y;
  i = 0;
  int totalCounter = 1 + 2*HEIGHT + 2*LENGTH;
  if(shop_bool){
    totalCounter += 28; 
  }    
                                       //totalCounter is total amount of entities on the map (non including "nothing" terrain)
                                               //have to take into account borders and block in middle   
  while(totalCounter < HEIGHT * LENGTH){ //while there is still room to grow
    i %= 5; //loop around to first terrain if i >= 5
    int counter;
    j = 0;
      counter = counters[i]; //save total amount of terrain i to start with
      while(j < counter){ //for every cell with terrain i to start with
        x = valid_x[i][j]; //grab the coords of cell
        y = valid_y[i][j];
        for(k = 0; k < MOVE_NUM; k++){ //go through all possible expansions of said cell
          if(x+move_x[k] < LENGTH && y + move_y[k] < HEIGHT && x+move_x[k] > 0 && y + move_y[k] > 0 && cells_i[y+move_y[k]][x + move_x[k]]->terr()== nothing){
            //only expand if the expansion is inside the map and a terrain (besides "nothing") is not already there
            cells_i[y+move_y[k]][x + move_x[k]]->set_terr(terrains[i]); // expand
            valid_x[i][counters[i]] = x+move_x[k]; //add to list of all cells with terrain i
            valid_y[i][counters[i]] = y+move_y[k];
            counters[i]++; //up number of cells with terrain i
            totalCounter++; //up total number of entities
          }
        }
        j++; //go to next cell
      } 
    i++; //go to next terrain
  }
  return true;
}

void convertPath(map *room, int *distance, int *x, int *y, int section, int xORy, int i){ //convert everything infront of current x or y to path
                                                                                            //for length of section. if xORy == 0, go up
                                                                                            //otherwise go accross
  int temp = distance[i]; //save distance to the block in middle
  while(xORy == 0 && absoluteValue(distance[i]) > 0 && section > 0){ //move vertical while the distance to the middle is not 0 and 
                                                                      //there is still section of path to put down
    room->cell(*x,*y + sign(temp))->set_terr(path);
    room->add_path(*x, *y + sign(temp));
    distance[i] -= sign(temp); //distance to middle gets closer of 0
    *y += sign(temp); //current y gets incremented
    section--; //1 less cell to put down in section
  }
  while(xORy == 1 && absoluteValue(distance[i]) > 0 && section > 0){ //move accross. otherwise same as above
    room->cell(*x + sign(temp),*y)->set_terr(path);
    room->add_path(*x + sign(temp), *y);
    distance[i] -= sign(temp);
    *x += sign(temp);
    section--;
  }
}

bool map::add_all_paths(bool shop_bool){ //bool is 1 if there are shops, 0 if no shops
  //srand(time(NULL)); 
  #define SEC_MAX 5 //maximum section length;
  
  int distAccross[4] = {0,0,(LENGTH/2 - 3), -(LENGTH/2 - 3)}; //distance accross and vertical from each entrance to center, + or - some offset to make it look better
  int distVert[4] = {HEIGHT/2-2, -(HEIGHT/2-1), 0,0};

  if(!shop_bool){ //if there are no shops, connect the paths all in the middle
    distVert[north] = HEIGHT/2;
    distVert[south] = -HEIGHT/2;
    distAccross[west] = LENGTH/2;
    distAccross[east] = -LENGTH/2;
  }
  int i, section;
  for(i = 0; i < 4; i++){ //caluculating distance to middle
    if(i < 2){
      if(!shop_bool){ 
        distAccross[i] = LENGTH/2 - entrances_i[i];
        continue;
      }
      distAccross[i] = LENGTH/2 - entrances_i[i] + 3;
      if(entrances_i[i] < LENGTH / 2){
        distAccross[i] -= 6;
      }
      if(i == (int)north && shop_bool) distAccross[i] -= sign(distAccross[i]); //offset only if there are shops to make it look better
    }
    else{
      if(!shop_bool){
        distVert[i] = HEIGHT/2 - entrances_i[i];
      }
      else if(entrances_i[i] < HEIGHT / 2){
        distVert[i] = HEIGHT/2 - entrances_i[i] - 2;
      }
      else{
        distVert[i] = HEIGHT/2 - entrances_i[i] +1;
      }
      if(i == (int)west && shop_bool) distVert[i] -= sign(distVert[i]);
    }
  }



  for (i = 0; i < 4; i++){ //if entrances_i[i] == -1, there is no valid entrance for that map
                          //make so paths are not made (dist to center = 0)
    if (entrances_i[i]<0){
      distAccross[i] = 0;
      distVert[i] = 0;
    }
  }


  int currentY = 0;
  int currentX = 0;
  for(i = 0; i < 4; i++){ //find BOTH coordinates for each entrance and have that as the current x and y
    int pivotCounter = 0;
    if(i > 1) pivotCounter +=1; //the first direction for north and south is vertical, accross for east and west
                                //done so the path can't interfere with the border
    switch(i){
      case north:  
        currentY = 0;
        currentX = entrances_i[i];
        break;
      case south:  
        currentY = HEIGHT - 1;
        currentX = entrances_i[i];
        break;
      case west:  
        currentY = entrances_i[i];
        currentX = 0;
        break;
      case east:  
        currentY = entrances_i[i];
        currentX = LENGTH - 1;
        break;
      default:
        printw("something went wrong");
        //return 1;
        break;
    }
    while(absoluteValue(distAccross[i]) > 0 && absoluteValue(distVert[i]) > 0){ //while the path still has a distance to go in both direction
      pivotCounter %= 2; //alternate between going vertical and horizontal
      section = rand() % SEC_MAX + 1;
      if(pivotCounter == 0){
        convertPath(this, distVert, &currentX, &currentY, section, pivotCounter, i); //go up
      }
      else{
        convertPath(this, distAccross, &currentX, &currentY, section, pivotCounter, i); //go accross
      }
      pivotCounter++;
    }
    if(distAccross[i] <= 0){ //if there is no more room to go accross, go straight vertical until hit middle
      while(absoluteValue(distVert[i]) > 0){
        cells_i[currentY + sign(distVert[i])][currentX]->set_terr(path);
        add_path(currentX, currentY + sign(distVert[i]));
        currentY += sign(distVert[i]);
        distVert[i] -= sign(distVert[i]);
      }
    }
    if(distVert[i] <=0){ //no more room vertical, go straight accross
      while(absoluteValue(distAccross[i]) > 0){
        cells_i[currentY][currentX + sign(distAccross[i])]->set_terr(path);
        add_path(currentX + sign(distAccross[i]), currentY);
        currentX += sign(distAccross[i]);
        distAccross[i] -= sign(distAccross[i]);
      }
    }
  }
  return true;
}

bool map::addDecor(){ //add a few boulders and trees into the map to make it look nicer
  int move_x[8] = {0,1,1,1,0,-1,-1,-1}; //every possible cell bordering 1 cell
  int move_y[8] = {1,1,0,-1,-1,-1,0,1};
  int i,j,x,y,tORb;
  int goAhead; //1 means we can have decor, 0 means no decor
  int numDecor = LENGTH * HEIGHT / 35; //number picked arbitrarily
  for (i = 0; i < numDecor; i++){
    goAhead = 0; //assume we can't put any decor (more on this later)
    x = rand() % (LENGTH - 2) + 1; //pick random cooridants for the decor (between all borders)
    y = rand() % (HEIGHT-2) + 1;
    if(cells_i[y][x]->terr()== path || cells_i[y][x]->terr()== center || cells_i[y][x]->terr()== market) continue; //make sure not to interfere with any paths or shops
    for(j = 0; j < 8; j++){
      if(cells_i[y + move_y[j]][x + move_x[j]]->terr()!= boulder && cells_i[y + move_y[j]][x + move_x[j]]->terr()!= bound) {
        goAhead = 1; //if all bordering cells are boulders, it looks weird if there is a tree there
                     //don't put any decor if surrounded by boulders
                    //if one bordering cell isn't a boulder, we are all good to go
        break;
      }
    }
    tORb = rand() % 2; //50% chance of boulder/tree. 
    if(goAhead == 1 && (cells_i[y][x]->terr()!= forest || tORb != 0)){
      if(tORb == 0) cells_i[y][x]->set_terr(tree);
      else cells_i[y][x]->set_terr(boulder);
    }
  }
  return true;
}

bool map::print_map(bool colors){
  int i, j;
  //make_color();
  for(i = 0; i < HEIGHT; i++){
    for(j = 0; j< LENGTH; j++){ //for every tile in the map
      if (j == pc_x_i && i == pc_y_i){
        // printf(" ");
      }
      cells_i[i][j]->print_map_cell(colors);
    }
    printw("\n");
  }
  printw("\n");
  return true;
}


