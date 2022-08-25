#include <iostream>
#include "character.h"
#include <stdexcept>

#define ABS_VAL(x) (x < 0) ? -x : x


// bool same_type(specific_poke& p, poke_move& move){
//     int index = p.id;
//     if(p.id > 898) index -=9102;
//         for (int i = index - 1; i < TOTAL_TYPES; i++){ //pokemon is guarenteed to be after entry index
//             pokemon_type& curr = all_pokemon_types[i];
//             if (p.id < curr.pokemon_id) continue;
//             else if(p.id > curr.pokemon_id) return false;
//             else if(p.type_id == curr.type_id || 
//             (i+1 < TOTAL_TYPES && all_pokemon_types[i+1].id == p.id && all_pokemon_types[i+1].type_id = p.type_id)) return true;
//         }
//     return false;
// }

// int damage(specific_poke& attack, specific_poke& defend, poke_move& move){
//     double crit = 1.0;
//     double stab = same_type(attack, move) ? 1.5 : 1.0;
//     double type = 1.0;

//     // if (move.type_id == poke.type)
//     if (rand()%256 < attack.get_base(SPEED) / 2) crit = 1.5;
//     double num = (((double)(2 * attack.get_level()) / 5) + 2) * move.power * ((double)attack.get_stat(ATT) / attack.get_stat(DEF));
//     return (num/50 + 2) * crit * ((rand()%16 + 85.0)/100.0) * stab * type;
// }

    character::character(){
        roster = new std::vector<specific_poke*>;
        // poke_counter = 0;
        type_i = hiker;
        dir_i = none;
        x_i = -1;
        y_i = -1;
        time_i = 0;
        defeated_i = false;
        // inventory;
        for (int i = 0; i < 3; i++) inventory[i] = 3;
        active_poke = NULL;
    }
    character::character(direction_t dir, character_t type, int x, int y){
        roster = new std::vector<specific_poke*>;
        time_i = 0;
        defeated_i = false;
        dir_i = dir;
        type_i = type;
        x_i = x;
        y_i = y;
        for (int i = 0; i < 3; i++) inventory[i] = 3;
        active_poke = NULL;
    }
    character::~character(){
        hn_i = NULL;
        //delete roster;
		for(int i = 0; i < (int)roster->size(); i++){
			if (roster->at(i)) delete roster->at(i);
		}
		delete roster;
		
		
    }

    int character::x(){
        return x_i;
    }
    int character::y(){
        return y_i;
    }
    direction_t character::dir(){
        return dir_i;
    }
    character_t character::type(){
        return type_i;
    }
    int character::time(){
        return time_i;
    }
    bool character::def(){
        // if (inventory[REVIVE - OFFSET] > 0) {
        //     defeated_i = false;
        //     return false;
        // }
        for (int i = 0; i < (int)roster->size(); i++){
            if(roster->at(i) != NULL && roster->at(i)->get_hp() > 0){
                defeated_i = false;
                return defeated_i;
            }
        }
        defeated_i = true;
        return defeated_i;
    }  
    heap_node_t *character::hn(){
        return hn_i;
    }

    specific_poke& character::get_poke(int i){
        if (i < 0 || i >= (int)roster->size()){
            throw new std::out_of_range("cannot access that pokemon");
        }
        return *roster->at(i);
    }

    // specific_poke** character::get_all_poke(){
    //     return roster;
    // }

    int character::get_num_poke(){
        return roster->size();
    }

    bool character::add_poke(specific_poke& new_poke){
        if (roster->size() > 5) return false;
        roster->push_back(&new_poke);
        // poke_counter++;
        return true;
    }

    int determine_level(int x, int y){
    int dist = ABS_VAL(x - 199) + ABS_VAL(y - 199);
    int min, max;
    if (dist < 4) return 1;
    if(dist <=200){
        min = 1;
        max = dist/2;
    }
    else{
        min = (dist-200) / 2;
        max = 100;
        if (min == 0) min++;
        else if (min >= max) return 100;
    }
    return rand()%(max + 1) + min;
    }

    bool character::gen_poke(int x, int y){
    // map* room = planet->mapn();
    // character* npc;
    // for (int j = 0; j < room->num_chars(); j++){
        // npc = room->get_char(j);
        this->add_poke(*(new specific_poke(determine_level(x, y))));
        for (int i = 1; i < 6; i++){
            int decide = rand()%10;
            if (decide >= 6) break;
            this->add_poke(*(new specific_poke(determine_level(x, y))));
        }
        return true;
    }



    bool character::delete_poke(int i){
        if (i >= (int)roster->size() || i < 0 || roster->at(i) == NULL) return false;
        delete roster->at(i);
        for (int j = i; j < 5; j++){
            roster->at(j) = roster->at(j+1);
        }
        roster->pop_back();
        // poke_counter--;
        return true;
    }

    bool character::set_x(int x){
        x_i = x;
        return true;
    }
    bool character::set_y(int y){
        y_i = y;
        return true;
    }
    bool character::set_dir(direction_t dir){
        dir_i = dir;
        return true;
    }
    bool character::set_type(character_t type){
        type_i = type;
        return true;
    }
    bool character::set_time(int time){
        time_i = time;
        return true;
    }
    bool character::add_time(int time){
        time_i += time;
        return true;
    }
    bool character::set_def(bool def){
        defeated_i = def;
        return true;
    }
    bool character::set_hn(heap_node_t *hn){
        hn_i = hn;
        return true;
    }

    bool character::commit_move(int i, specific_poke& poke, bool wild, int move_choice){
        if (i == POTION || i == REVIVE || i == BALL){
            return use_item(i, wild, active_poke);
        }
        if (i == CHANGE){
            for(int i = 0 ; i <(int) roster->size(); i++){
                if (roster->at(i)->get_hp()) {
                    active_poke = roster->at(i);
                    return true;
                }
            }
        }
        if (i == FIGHT){
            // int choose = 0;
            if (active_poke->get_move(move_choice).id == -1) return false;
            int chance = rand()%100;
            if(chance >= active_poke->get_move(move_choice).accuracy) return false;
            poke.get_hit(*active_poke, active_poke->get_move(move_choice));
            return true;
        }
        return false;
    }

    int character::choose_move(){ //only to be used by the AI
        // bool item = false;
        if (def() || active_poke == NULL) return -1;

        if (active_poke->get_stat(HP) - active_poke->get_hp() >= 20 && active_poke->get_hp() != 0 && get_item(POTION)){
            return POTION;
        }
        // if (active_poke->get_hp() == 0 && get_item(REVIVE)){
        //     return REVIVE;
        // }
        if (active_poke->get_hp() == 0){
            for (int i = 0; i < (int)roster->size(); i++){
                if (roster->at(i)->get_hp()) {
                    // active_poke = roster->at(i);
                    return CHANGE;
                }
            }
        }
        return FIGHT;
        // return 0;
    }
    
    bool character::use_item(int i, bool wild, specific_poke* poke){
        if (i < 4 || i >= 7 || active_poke == NULL) return false;
        if (inventory[i - OFFSET] <= 0) return false;
        if (i == REVIVE){
            if (active_poke->get_hp() != 0) return false;
            active_poke->set_hp(active_poke->get_stat(HP) / 2);
        }
        else if(i == POTION){
            active_poke->set_hp(active_poke->get_hp() + 20);
        }
        else if (i == BALL){
            if (!wild) return false;
            if (roster->size() >= 6) return false;
            add_poke(*poke);
        }
        i -= OFFSET;
        inventory[i]--;
        return true;
    }
    int character::get_item(int i){
        i -= OFFSET;
        if (i < 0 || i >= 3) return -1;
        return inventory[i];
    }
    int* character::get_inventory(){
        return inventory;
    }

    bool character::set_active(int i){
        if (i < 0 || i >= (int)roster->size()) return false;
        this->active_poke = roster->at(i);
        return true;
    }

    bool character::set_active(specific_poke& poke){
        this->active_poke = &poke;
        return true;
    }

    specific_poke* character::get_active(){
        return active_poke;
    }

    // character &character::operator=(const  character &c){
    //     // type_i = c.type();
    //     // dir = c.dir();
    //     // x_i = c.x();
    //     // y_i = c.y();
    //     // defeated_i = c.def();
    //     // time_i = c.time();
    //     // return *this;
    //     return NULL;
    // }
    // std::ostream &operator<<(std::ostream &o, const character &c){
    // // char *defeat_state[2] = {(char*)"Not defeated", (char*)"Defeated"};
    // // char *types[7] = {(char*)"Hiker,        ", (char*)"Rival,        ", (char*)"Player,        ", (char*)"Pacer,        ", 
    // // (char*)"Wanderer,     ", (char*)"Stationary,   ", (char*)"Random Walker,"};
    // // char *def = defeat_state[0];
    // // if (c.defeated){
    // //     def = defeat_state[1];
    // // }
    // // return o << types[c.type_i] << c.x_i() << ", " << c.y_i() << ", " << def << std::endl; 
    // return NULL;
    // }
