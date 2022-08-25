#include "path_finding.h"


character_t current_person;

int32_t compare_map_cells(const void *key, const void *with) //compare distances to player of two map cells
{
    map_cell *key_n = (map_cell *)key;
    map_cell *with_n = (map_cell *)with;

    int keyDist = key_n->dist(current_person);
    int withDist = with_n->dist(current_person);
  return keyDist -  withDist;
}


// int print_paths_dist(map_t *room, character_t person){
//   int i,j, current;
//   for(i = 0; i < HEIGHT; i++){
//       for (j = 0; j < LENGTH; j++){
//           current = room->map[i][j]->dist[person];
//           switch((current / 100) % 6 ){ //switch the color of the display depending on how far away the cell is
//             case 0:
//               printf("\033[0;37m"); //white (within 100 / 700)
//               break;
//             case 1:
//               printf("\033[0;32m"); //green (within 200 / 800)
//               break;             
//             case 2:
//               printf("\033[0;33m"); //yellow (within 300 / 900)
//               break;
//             case 3:
//               printf("\033[0;31m"); //red (within 400 / 1000)
//               break;
//             case 4:
//               printf("\033[0;35m"); //magenta (within 500 / 1100)
//               break;
//             case 5:
//               printf("\033[0;36m"); //cyan (within 600 / 1200)
//               break;
//             default:
//               printf("\033[0;37m"); 
//               break;
//           } 
//           switch(current){
//             case 0:
//               printf("\033[0;34m");
//               printf("%02d", current);
//               break;
//             case INT_MAX:
//               printf("  ");
//               break;
//             default:
//               printf("%02d", current % 100);
//               break;
//           }
//       }
//       printf("\n");
//   }
//   printf("\033[0;37m");
//   return 0;
// }

// int print_all_maps(world_t *world){
//   int realX = world->X - 199;
//   int realY = world->Y - 199;

//   printf("Hiker Distance Map:\n");
//   print_paths_dist(world->maps[world->Y][world->X], hiker);
//   printf("Rival Distance Map:\n");
//   print_paths_dist(world->maps[world->Y][world->X], rival);
//   printf("Terrain Map:\n");
//   print_map(world->maps[world->Y][world->X]);
//   printf("%d, %d\n", realX, realY);
//   return 0;

// }

int place_pc(map *room){ 
    int index = rand() % room->num_path();//choose a random cell in the paths_x and paths_y array
    room->set_pc(room->path_x(index), room->path_y(index));
    return 0;
}

int calculate_all_paths(map *room, character_t person){
  current_person = person;
  const int MOVE_X[8] = {0,1,1,1,0,-1,-1,-1}; 
  const int MOVE_Y[8] = {1,1,0,-1,-1,-1,0,1};
  int i,j;
  int x_inc, y_inc;
  heap_t h;
  heap_init(&h, compare_map_cells, NULL);
  int done[HEIGHT][LENGTH];

  for (i = 0; i < HEIGHT; i++){ //initialize all the terrain costs to the right value and distances to 0
                                //and initialize all nodes to not done 
      for (j =0; j < LENGTH; j++){
        room->cell(j,i)->set_dist(person, INT_MAX);
        done[i][j] = 0;
      }
  }
  room->pc_cell()->set_dist(person, 0); //player distance is 0
  room->pc_cell()->set_hn( heap_insert(&h, room->pc_cell()));
  map_cell *u, *neighbor;

  while((u = (map_cell*)heap_remove_min(&h))){
      u->set_hn(NULL);
      // u->done[person] = 1;
      done[u->y()][u->x()] = 1;
      for(i = 0; i < 8; i++){
          x_inc = MOVE_X[i];
          y_inc = MOVE_Y[i];
          if(u->x() + x_inc >=LENGTH || u->x() + x_inc < 0 || u->y() + y_inc >= HEIGHT || u->y() + y_inc <0) continue; //make sure the value isn't out of bounds
          neighbor = room->cell(u->x()+x_inc, u->y() + y_inc);
          if(!done[neighbor->y()][neighbor->x()] && u->cost(person) < INT_MAX && neighbor->cost(person) < INT_MAX && neighbor->dist(person) >  u->dist(person) + u->cost(person)){
              //A lot going on in this if statement. 1: neighbor has to be unvisited. 2: both u and neighbor must be passable. Otherwise no point in adding it to the heap because
              //the character will never go there. 3:neighbor's path through u must be less than its current distance (dijkstra's alg).
              neighbor->set_dist(person, u->dist(person) + u->cost(person));
              neighbor->set_hn(heap_insert(&h, neighbor));

          }
      }
  }

  heap_delete(&h);
  return 0;

}




