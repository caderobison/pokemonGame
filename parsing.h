#ifndef PARSING_H
#define PARSING_H
#include <string>
#include <iostream>
// #include "user_interface.h" 
#include <fstream>
#include <ncurses.h>

#define NUM_MOVES 2000
#define TOTAL_MOVES 528238
#define TOTAL_TYPES 1675
#define TOTAL_STAT 6552
#define NUM_STATS 6
#define HP 0
#define ATT 1
#define DEF 2
#define SATT 3
#define SDEF 4
#define SPEED 5
#define MAX_IV 16

#define NUM_POKE 1092


class pokemon;
class poke_move;
class pokemon_move;
class stat;
class pokemon_species;
class experience;
class type_name;
class specific_poke;
class pokemon_type;

extern pokemon *all_pokemon;
extern poke_move *all_moves;
extern pokemon_move *all_pokemon_moves;
extern stat *all_stat;
extern pokemon_species *all_species;
extern experience *all_exp;
extern type_name *all_types;
extern pokemon_type* all_pokemon_types;

class pokemon{
    public:
        int id;
        std::string name;
        int species;
        int height;
        int weight;
        int base_exp;
        int order;
        int is_default;
        
        pokemon(int id, std::string name, int species, int height, int weight, int base_exp, int order, int is_default);
        pokemon();
        ~pokemon();
        pokemon(pokemon& poke);

        void set_pokemon(int id, std::string name, int species, int height, int weight, int base_exp, int order, int is_default);

        friend std::ostream &operator<<(std::ostream &o, const pokemon &p);
        bool operator==(pokemon&);
        bool operator!=(pokemon&);
        // friend pokemon &operator=(const pokemon &p);

};

// inline pokemon &operator=(const pokemon &p){
//     this->set_pokemon(p.id, p.name, p.spicies, p.height, p.weight, p.base_exp, p.order, p.is_default);
//     return *this;
// }


class poke_move{
    public:
    int id;
    std::string identifier; 
    int generation_id; 
    int type_id; 
    int power; 
    int pp; 
    int accuracy; 
    int priority; 
    int target_id; 
    int damage_class_id; 
    int effect_id; 
    int effect_chance; 
    int contest_type_id; 
    int contest_effect_id; 
    int super_contest_effect_id;

        
    poke_move(int id, std::string identifier, int generation_id, int type_id, 
    int power, int pp, int accuracy, int priority, int target_id, int damage_class_id, int effect_id, int effect_chance,
        int contest_type_id, int contest_effect_id, int super_contest_effect_id);
    poke_move();
    // poke_move(poke_move&);
    ~poke_move();
    void set_poke_move(int id, std::string identifier, int generation_id, int type_id, 
    int power, int pp, int accuracy, int priority, int target_id, int damage_class_id, int effect_id, int effect_chance,
        int contest_type_id, int contest_effect_id, int super_contest_effect_id);

    void copy_poke_move(poke_move& p);

    friend std::ostream &operator<<(std::ostream &o, const poke_move &p);
    poke_move &operator=(const poke_move &p);


};

class pokemon_move{
    public:
    int pokemon_id,version_group_id,move_id,pokemon_move_method_id,level,order;

        pokemon_move(int pokemon_id, int version_group_id, int move_id, int pokemon_move_method_id, int level, int order);
        pokemon_move();
        // pokemon_move(const pokemon_move&);
        void set_pokemon_move(int pokemon_id, int version_group_id, int move_id, int pokemon_move_method_id, int level, int order);
        void copy_pokemon_move(pokemon_move& p);
        ~pokemon_move();
        pokemon_move &operator=(const pokemon_move &p);
        friend std::ostream &operator<<(std::ostream &o, const pokemon_move &p);

};



class pokemon_species{
    public:
        int id,generation_id,evolves_from_species_id,evolution_chain_id,color_id,
        shape_id,habitat_id,gender_rate,capture_rate,base_happiness,
        is_baby,hatch_counter,has_gender_differences,
        growth_rate_id,forms_switchable,is_legendary,is_mythical,order,conquest_order;
        std::string identifier;
        
        pokemon_species(int id, std::string identifier, int generation_id, int evolves_from_species_id, int 
        evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int 
        capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int 
        growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order);
        pokemon_species();
        ~pokemon_species();
        void set_pokemon_species(int id, std::string identifier, int generation_id, int evolves_from_species_id, int 
        evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int 
        capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int 
        growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order);

        friend std::ostream &operator<<(std::ostream &o, const pokemon_species &p);

};

class experience{
    public:
        int growth_rate_id,level,experience_i;        
        experience(int growth_rate_id, int level, int experience_i);
        experience();
        void set_experience(int growth_rate_id, int level, int experience_i);
        ~experience();
        friend std::ostream &operator<<(std::ostream &o, const experience &p);
};

class type_name{
    public:
        int type_id,local_language_id;
        std::string name;

        type_name(int type_id, int local_language_id, std::string name);
        type_name();
        void set_type_name(int type_id, int local_language_id, std::string name);
        ~type_name();
        friend std::ostream &operator<<(std::ostream &o, const type_name &p);

};

class stat{
    public:
        int pokemon_id,stat_id,base_stat,effort;

        stat(int pokemon_id, int stat_id, int base_stat, int effort);
        stat();
        void set_stat(int pokemon_id, int stat_id, int base_stat, int effort);
        ~stat();
        friend std::ostream &operator<<(std::ostream &o, const stat &p);
};

pokemon* make_pokemon(std::ifstream &, int);
poke_move* make_moves(std::ifstream &, int);
pokemon_move* make_pokemon_moves(std::ifstream &, int);
pokemon_species* make_pokemon_species(std::ifstream &, int);
experience* make_experience(std::ifstream &, int);
type_name* make_type_name(std::ifstream &, int);
stat* make_stat(std::ifstream &, int);
pokemon_type* make_pokemon_type(std::ifstream &, int);
void* parse_file(std::string);

// static poke_move &find_move(pokemon_move this_move, poke_move* move_types){
//     move(23, 0);
//     printw("%d", this_move.move_id);
//     move(0, 0);
//     refresh();
//     if (this_move.move_id == -1) return *(new poke_move());
//     else if (this_move.move_id <= 826) return move_types[this_move.move_id - 1];
//     else return move_types[this_move.move_id - 9174];
// }

class pokemon_type{
    public:
        int pokemon_id, type_id, slot;

        pokemon_type(int pokemon_id, int type_id, int slot);
        pokemon_type();
        void set_pokemon_type(int pokemon_id, int type_id, int slot);
        ~pokemon_type();
        friend std::ostream &operator<<(std::ostream &o, const pokemon_type &p);

};


class specific_poke : public pokemon{
private:
    pokemon_move **move_stat_arr;
    poke_move **move_arr;
    poke_move *null_move;
    pokemon_move *null_stat;
    int level;
    int iv[6];
    int stats[6];
    int base[6];
    bool gender; //true is male, false is female
    bool shiny;
    int hp;
    bool init_poke( int);
public:
    // specific_poke(specific_poke&);
    specific_poke();
    specific_poke(/*pokemon* all_poke*/ int level /*pokemon_move* all_moves, poke_move* move_types, stat* all_stat*/); //generates moves and stats, chooses all_poke[i];
    specific_poke(pokemon& poke, int level/*, pokemon_move* all_moves, poke_move* move_types, stat* all_stat*/); //generates moves + stats for already created pokemon
    // specific_poke(pokemon &poke, int level); //doesn't generate moves or stats
    ~specific_poke();
    pokemon_move& get_move_stat(int i);
    poke_move& get_move(int i);
    int get_level();
    bool set_level(int level);
    bool level_up();

    pokemon_move ** get_move_stat_arr();
    poke_move ** get_move_arr();


    bool gen_stats();
    int get_stat(int);
    int get_base(int);
    int get_iv(int);
    // bool get_hit();
    bool get_gender();
    int get_hp();
    bool set_hp(int);

    bool is_shiny();

    bool gen_moves();
    bool get_hit(specific_poke& attack, poke_move& p_move);

    bool operator==(specific_poke&);
    bool operator!=(specific_poke&);

};
#endif