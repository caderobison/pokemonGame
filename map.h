#ifndef MAP_H
#define MAP_H

#include "map_cell.h"


#define LENGTH  80
#define HEIGHT  21
#define NUM_CHAR_TYPES 7

class map{ //Structure that houses the map and info about the entrances
    private:
        int x;
        int y;
        map_cell* cells_i[HEIGHT][LENGTH];
        int entrances_i[4]; //holds the relevant coordinate for each entrance. 0 is north, 1 is south, 2 is west, 3 is east. 
                        //"relevant coordinate" is the only coordinate that is random. e.g. entrances[0] and entrances[1] will hold the x
                        //coordinate as the y cooridnate is always at 0 for north and HEIGHT-1 for south. 
                        //entrances[2] and entrances[3] hold y coordinates
        int paths_x_i[2*(HEIGHT + LENGTH)]; //xcoords for all the paths in the map
        int paths_y_i[2*(HEIGHT + LENGTH)]; //ycoords for all the paths in the map
        int num_path_i; //number of path blocks in the 
        int pc_x_i; //xcoord of player
        int pc_y_i; //ycoord of player
        character** all_chars_i;
        int num_chars_i;
        heap_t *char_heap_i;
        bool init_map();

    public:
        map();
        map(int x, int y);
        ~map();

        int get_x();
        int get_y();
        bool set_coords(int, int);

        map_cell* cell(int, int);
        map_cell* pc_cell();
        int entrance(direction_t);
        //int[4] entrance_arr();
        int path_x(int);
        int path_y(int);
        int num_path();
        int pc_x();
        int pc_y();
        int num_chars();
        character* get_char(int);
        character** all_chars();
        heap_t* char_heap();

        bool add_path(int, int);
        bool add_all_paths(bool);
        bool set_pc(int, int);
        bool set_num_chars(int);
        bool add_char(character*);
        bool set_char(character*, int);
        bool set_char_heap(heap_t*);
        bool remove_char_heap();

        bool add_terrain_cost();
        // int delete_room(map_t*); //free all map_cells
        bool addBorders(); //add boulders to borders, no entrances yet
        bool print_map(bool); //print the map
        bool add_one_entrance(direction_t, int);
        // bool addEntrance(); //add ALL entrances to the map
        bool addShops(); //add the center and market and paths around them to mao
        bool growRegion(bool); //seed regions and grow outward 
        bool addPaths(bool); // add paths from entrances to center + market
        bool addDecor(); //add some boulders and trees dispersed accross the map

        // int absoluteValue(int);
        // int make_color(int);
};

#endif