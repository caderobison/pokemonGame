#include "parsing.h"
#include <cstring>
#include <vector>

#define MIN(x,y) ((x < y) ? x : y)
#define MAX(x,y) ((x > y) ? x : y)



pokemon::pokemon(int id, std::string name, int species, int height, int weight, int base_exp, int order, int is_default){
    this->id = id;
    this->name = name;
    this->species = species;
    this->height = height;
    this->weight = weight;
    this->base_exp = base_exp;
    this->order = order;
    this->is_default = is_default;

    }
pokemon::pokemon(){
        this->set_pokemon(-1,"", -1,-1,-1,-1,-1,-1);
    }

pokemon::pokemon(pokemon& p){
        this->set_pokemon(p.id,p.name, p.species,p.height,p.weight,p.base_exp,p.order,p.is_default);
    }

pokemon::~pokemon(){}

void pokemon::set_pokemon(int id, std::string name, int species, int height, int weight, int base_exp, int order, int is_default){
    this->id = id;
    this->name = name;
    this->species = species;
    this->height = height;
    this->weight = weight;
    this->base_exp = base_exp;
    this->order = order;
    this->is_default = is_default;

}

bool pokemon::operator==(pokemon& p){
    return id == p.id && name == p.name && species == p.species && height == p.height && weight == p.weight && base_exp == p.base_exp
    && order == p.order && is_default == p.is_default;
}

bool pokemon::operator!=(pokemon& p){
    return !(*this == p);
}

        // friend pokemon &operator=(const pokemon &p);

inline std::ostream &operator<<(std::ostream &o, const pokemon &p){
   
    return o <<p.id<< " , " <<p.name<< " , " <<p.species<< " , " <<p.height<< " , " <<p.weight<< " , " <<p.base_exp<< " , " 
    <<p.order<< " , " <<p.is_default<< std::endl; 
}

// inline pokemon &operator=(const pokemon &p){
//     this->set_pokemon(p.id, p.name, p.spicies, p.height, p.weight, p.base_exp, p.order, p.is_default);
//     return *this;
// }


        
poke_move::poke_move(int id, std::string identifier, int generation_id, int type_id, 
int power, int pp, int accuracy, int priority, int target_id, int damage_class_id, int effect_id, int effect_chance,
    int contest_type_id, int contest_effect_id, int super_contest_effect_id){

this->id = id;
this->identifier = identifier;
this->generation_id = generation_id;
this->type_id = type_id;
this->power = power;
this->pp = pp;
this->accuracy = accuracy;
this->priority = priority;
this->target_id = target_id;
this->damage_class_id = damage_class_id;
this->effect_id = effect_id;
this->effect_chance = effect_chance;
this->contest_type_id = contest_type_id;
this->contest_effect_id = contest_effect_id;
this->super_contest_effect_id = super_contest_effect_id;

}
poke_move::poke_move(){
    this->set_poke_move(-1,"", -1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1);
    // this->id = -1;
    // this->identifier = "";
    // this->generation_id = -1;
    // this->type_id = -1;
    // this->power = -1;
    // this->pp = -1;
    // this->accuracy = -1;
    // this->priority = -1;
    // this->target_id = -1;
    // this->damage_class_id = -1;
    // this->effect_id = -1;
    // this->effect_chance = -1;
    // this->contest_type_id = -1;
    // this->contest_effect_id = -1;
    // this->super_contest_effect_id = -1;
}

poke_move::~poke_move(){}

void poke_move::set_poke_move(int id, std::string identifier, int generation_id, int type_id, 
int power, int pp, int accuracy, int priority, int target_id, int damage_class_id, int effect_id, int effect_chance,
    int contest_type_id, int contest_effect_id, int super_contest_effect_id){

this->id = id;
this->identifier = identifier;
this->generation_id = generation_id;
this->type_id = type_id;
this->power = power;
this->pp = pp;
this->accuracy = accuracy;
this->priority = priority;
this->target_id = target_id;
this->damage_class_id = damage_class_id;
this->effect_id = effect_id;
this->effect_chance = effect_chance;
this->contest_type_id = contest_type_id;
this->contest_effect_id = contest_effect_id;
this->super_contest_effect_id = super_contest_effect_id;

}

void poke_move::copy_poke_move(poke_move& p){
    this->set_poke_move(p.id, p.identifier, p.generation_id, p.type_id, p.power, p.pp, p.accuracy, p.priority, p.target_id
    , p.damage_class_id, p.effect_id, p.effect_chance, p.contest_type_id, p.contest_effect_id, p.super_contest_effect_id);
}

poke_move &poke_move::operator=(const poke_move &p){
    this->set_poke_move(p.id, p.identifier, p.generation_id, p.type_id, p.power, p.pp, p.accuracy, p.priority, p.target_id
    , p.damage_class_id, p.effect_id, p.effect_chance, p.contest_type_id, p.contest_effect_id, p.super_contest_effect_id);
    return *this;
}


inline std::ostream &operator<<(std::ostream &o, const poke_move &p){
   
    return o <<p.id<< " , " <<p.identifier<< " , " <<p.generation_id<< " , " <<p.type_id<< " , " <<p.power<< " , " <<p.pp<< " , " <<p.accuracy<< " , " <<
    p.priority<< " , " <<p.target_id<< " , " <<p.damage_class_id<< " , " <<p.effect_id<< " , " <<p.effect_chance<< " , " <<
    p.contest_type_id<< " , " <<p.contest_effect_id<< " , " <<p.super_contest_effect_id << std::endl; 
}

pokemon_move::pokemon_move(int pokemon_id, int version_group_id, int move_id, int pokemon_move_method_id, int level, int order){

this->pokemon_id = pokemon_id;
this->version_group_id = version_group_id;
this->move_id = move_id;
this->pokemon_move_method_id = pokemon_move_method_id;
this->level = level;
this->order = order;

}
pokemon_move::pokemon_move(){
    this->set_pokemon_move(-1,-1,-1,-1,-1,-1);
    // this->pokemon_id = -1;
    // this->version_group_id = -1;
    // this->move_id = -1;
    // this->pokemon_move_method_id = -1;
    // this->level = -1;
    // this->order = -1;
}

void pokemon_move::set_pokemon_move(int pokemon_id, int version_group_id, int move_id, int pokemon_move_method_id, int level, int order){

this->pokemon_id = pokemon_id;
this->version_group_id = version_group_id;
this->move_id = move_id;
this->pokemon_move_method_id = pokemon_move_method_id;
this->level = level;
this->order = order;

}

void pokemon_move::copy_pokemon_move(pokemon_move& p){
    this->set_pokemon_move(p.pokemon_id, p.version_group_id, p.move_id, p.pokemon_move_method_id, p.level, p.order);
}

pokemon_move::~pokemon_move(){}
pokemon_move &pokemon_move::operator=(const pokemon_move &p){
    this->set_pokemon_move(p.pokemon_id, p.version_group_id, p.move_id, p.pokemon_move_method_id, p.level, p.order);
    return *this;
}

inline std::ostream &operator<<(std::ostream &o, const pokemon_move &p){
   
    return o <<p.pokemon_id << ", "<< p.version_group_id<< ", " <<  p.move_id<< ", "  << p.pokemon_move_method_id<< ", "  << p.level<< ", "  << p.order<< ", "  << std::endl; 
}



pokemon_species::pokemon_species(int id, std::string identifier, int generation_id, int evolves_from_species_id, int 
evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int 
capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int 
growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order){
    
    this->id = id;
    this->identifier = identifier;
    this->generation_id = generation_id;
    this->evolves_from_species_id = evolves_from_species_id;
    this->evolution_chain_id = evolution_chain_id;
    this->color_id = color_id;
    this->shape_id = shape_id;
    this->habitat_id = habitat_id;
    this->gender_rate = gender_rate;
    this->capture_rate = capture_rate; 
    this->base_happiness = base_happiness;
    this->is_baby = is_baby;
    this->hatch_counter=hatch_counter;
    this->has_gender_differences = has_gender_differences; 
    this->growth_rate_id = growth_rate_id;
    this->forms_switchable = forms_switchable;
    this->is_legendary = is_legendary;
    this->is_mythical = is_mythical;
    this->order = order;
    this->conquest_order = conquest_order;


}
pokemon_species::pokemon_species(){
    this->set_pokemon_species(-1,(std::string)"", -1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1);
}
pokemon_species::~pokemon_species(){}

void pokemon_species::set_pokemon_species(int id, std::string identifier, int generation_id, int evolves_from_species_id, int 
evolution_chain_id, int color_id, int shape_id, int habitat_id, int gender_rate, int 
capture_rate, int base_happiness, int is_baby, int hatch_counter, int has_gender_differences, int 
growth_rate_id, int forms_switchable, int is_legendary, int is_mythical, int order, int conquest_order){
    
    this->id = id;
    this->identifier = identifier;
    this->generation_id = generation_id;
    this->evolves_from_species_id = evolves_from_species_id;
    this->evolution_chain_id = evolution_chain_id;
    this->color_id = color_id;
    this->shape_id = shape_id;
    this->habitat_id = habitat_id;
    this->gender_rate = gender_rate;
    this->capture_rate = capture_rate; 
    this->base_happiness = base_happiness;
    this->is_baby = is_baby;
    this->hatch_counter=hatch_counter;
    this->has_gender_differences = has_gender_differences; 
    this->growth_rate_id = growth_rate_id;
    this->forms_switchable = forms_switchable;
    this->is_legendary = is_legendary;
    this->is_mythical = is_mythical;
    this->order = order;
    this->conquest_order = conquest_order;


}

inline std::ostream &operator<<(std::ostream &o, const pokemon_species &p){
   return o <<p.id << ", "<<p.identifier<< ", "<<p.generation_id<<", "<<p.evolves_from_species_id<< ", "<<p.evolution_chain_id<< ", "<<p.color_id<< ", "<<p.shape_id<< ", "<<p.habitat_id<< ", "<<p.gender_rate<< ", "<<p.capture_rate<< ", "<<p.base_happiness<< ", "<<p.is_baby<< ", "<<p.hatch_counter<< ", "<<p.has_gender_differences<< ", "<<p.growth_rate_id<< ", "<<p.forms_switchable<< ", "<<p.is_legendary<< ", "<<p.is_mythical<< ", "<<p.order<< ", "<<p.conquest_order << std::endl;
            
}

experience::experience(int growth_rate_id, int level, int experience_i){
    this->growth_rate_id = growth_rate_id;
    this->level = level;
    this->experience_i = experience_i;
}
experience::experience(){
    this->set_experience(-1,-1,-1);
}

void experience::set_experience(int growth_rate_id, int level, int experience_i){
    this->growth_rate_id = growth_rate_id;
    this->level = level;
    this->experience_i = experience_i;
}

experience::~experience(){}


inline std::ostream &operator<<(std::ostream &o, const experience &p){
   
    return o<<p.growth_rate_id<<", " <<p.level << ", " << p.experience_i<< std::endl; 
}



        
type_name::type_name(int type_id, int local_language_id, std::string name){
    this->type_id = type_id;
    this->local_language_id = local_language_id;
    this->name = name;
}
type_name::type_name(){
    this->set_type_name(-1,-1,(std::string)"");
}

void type_name::set_type_name(int type_id, int local_language_id, std::string name){
    this->type_id = type_id;
    this->local_language_id = local_language_id;
    this->name = name;
}

type_name::~type_name(){}


inline std::ostream &operator<<(std::ostream &o, const type_name &p){
   
    return o<<p.type_id<<", " <<p.local_language_id << ", " << p.name<< std::endl; 
}

stat::stat(int pokemon_id, int stat_id, int base_stat, int effort){
    this->pokemon_id = pokemon_id;
    this->stat_id = stat_id;
    this->base_stat = base_stat;
    this->effort = effort;
}
stat::stat(){
    this->set_stat(-1,-1,-1, -1);
}

void stat::set_stat(int pokemon_id, int stat_id, int base_stat, int effort){
    this->pokemon_id = pokemon_id;
    this->stat_id = stat_id;
    this->base_stat = base_stat;
    this->effort = effort;
}

stat::~stat(){}


inline std::ostream &operator<<(std::ostream &o, const stat &p){
   
    return o<<p.pokemon_id<<", " <<p.stat_id << ", " << p.base_stat<< ", " << p.effort <<  std::endl; 
}





pokemon_type::pokemon_type(int pokemon_id, int type_id, int slot){
    this->pokemon_id = pokemon_id;
    this->type_id = type_id;
    this->slot = slot;
}
pokemon_type::pokemon_type(){
    this->set_pokemon_type(-1,-1,-1);
}
void pokemon_type::set_pokemon_type(int pokemon_id, int type_id, int slot){
    this->pokemon_id = pokemon_id;
    this->type_id = type_id;
    this->slot = slot;
}
pokemon_type::~pokemon_type(){}

inline std::ostream& operator<<(std::ostream &o, const pokemon_type& p){
    return o << p.pokemon_id << ", " << p.type_id << ", " << p.slot << std::endl;
}

//----------------------------------------------------------------------------------------------------------}--------------


static poke_move &find_move(pokemon_move &this_move){
    // move(23, 0);
    // printw("%d", this_move.move_id);
    // move(0, 0);
    //refresh();
    // if (this_move.move_id == -1) return *(new poke_move());
    if (this_move.move_id <= 826) return all_moves[this_move.move_id - 1];
    else return all_moves[this_move.move_id - 9174];
}
bool specific_poke::init_poke(int level){
    this->null_move = new poke_move();
    this->null_stat = new pokemon_move();
    this->level = level;
    this->gender = (rand()%2 == 0);
    this->shiny = (rand()%8192 == 0);
    for(int i = 0; i < 6; i++){
        this->iv[i] = rand()%MAX_IV;
    }
    this->move_stat_arr = new pokemon_move*[2];
    this->move_arr = new poke_move*[2];
    gen_moves();
    gen_stats();
    return true;
}

// specific_poke::specific_poke(specific_poke& p) : pokemon(p.id,p.name, p.species,p.height,p.weight,p.base_exp,p.order,p.is_default){
//     init_poke(p.get_level());

//     this->hp = p.get_hp();
//     this->level = p.get_level();
//     null_move = new poke_move();
//     null_stat = new pokemon_move();
//     gender = p.get_gender();
//     shiny = p.is_shiny();
//     move_stat_arr = new pokemon_move*[2];
//     move_arr = new poke_move*[2];
//     for (int i = 0; i < 6; i++){
//         if (i < 2){
//             move_arr[i] = new poke_move(p.get_move(i));
//             move_stat_arr[i] = new pokemon_move(p.get_move_stat(i));
//         }
//         this->stats[i] = p.get_stat(i);
//         base[i] = p.get_base(i);
//         iv[i] = p.get_iv(i);
//     }
// }

// specific_poke::specific_poke(pokemon &poke, int level) : pokemon(poke){
//     // specific_poke(poke,level) : pokemon(poke);
//     init_poke(level);
//     this->hp = stats[HP];
// }

specific_poke::specific_poke(/*pokemon* all_poke,*/ int level/*, pokemon_move* all_moves, poke_move* move_types, stat* all_stat*/) : pokemon(all_pokemon[rand()%NUM_POKE]){
    // specific_poke(poke,level) : pokemon(poke);
    init_poke(level);
    
    // gen_moves(all_moves, move_types);
    // gen_stats(all_stat);
    this->hp = stats[HP];
}

specific_poke::specific_poke() : pokemon(all_pokemon[rand()%NUM_POKE]){
    init_poke(1);
    this->hp = stats[HP];

}

specific_poke::specific_poke(pokemon& poke, int level/*, pokemon_move* all_moves, poke_move* move_types, stat* all_stat*/) : pokemon(poke){
    // specific_poke(poke,level) : pokemon(poke);
    init_poke(level);
    
    // gen_moves(all_moves, move_types);
    // gen_stats(all_stat);
    this->hp = stats[HP];
}
int specific_poke::get_hp(){
    return hp;
}

bool specific_poke::set_hp(int health){
    if (stats[HP] == -1 || health < 0) return false;
    hp = MIN(stats[HP], health);
    return true;
}


specific_poke::~specific_poke(){
    delete null_move; 
    delete null_stat;
    delete[] move_arr;
    delete[] move_stat_arr;
}
pokemon_move& specific_poke::get_move_stat(int i){
    if (i >= 2 || i < 0) return *null_stat;
    return *move_stat_arr[i];
}
poke_move& specific_poke::get_move(int i){
    if (i >= 2 || i < 0 || move_arr[i] == NULL) return *null_move;
    return *move_arr[i];
}

int specific_poke::get_level(){
    return level;
}

pokemon_move** specific_poke::get_move_stat_arr() {return move_stat_arr;}
poke_move** specific_poke::get_move_arr(){return move_arr;}

bool specific_poke::set_level(int level){
    this->level = level;
    return true;
}


bool specific_poke::gen_stats(){
    stat poke_stats[NUM_STATS];
    for(int i = 0; i < TOTAL_STAT; i++){
        if (all_stat[i].pokemon_id == id && all_stat[i].stat_id < 7){
            poke_stats[all_stat[i].stat_id - 1] = all_stat[i];
        }
    }
    for (int i = 0; i < NUM_STATS; i++){
        stats[i] = poke_stats[i].base_stat + iv[i];
        base[i] = poke_stats[i].base_stat;
    }
    return true;
}

bool specific_poke::level_up(){
    if (level >= 100) return false;
    level++;
    // stats[HP] = (((base[HP] + iv[HP]) * 2 * level)/100) + level + 10;
    for (int i = 0; i < 6; i++){
        stats[i] += (((base[HP] + iv[HP]) * 2 * level)/100) + 5;
        if (i == HP) stats[i] += (5 + level); 
    }
    return true;
}

// bool specific_poke::hit(int dp){
//     hp = MAX(0,hp - dp);
//     return true;
// }

int specific_poke::get_stat(int index){
    if (index >= NUM_STATS|| index < 0) return -1;
    return stats[index];
}

int specific_poke::get_base(int index){
    if (index >= NUM_STATS || index < 0) return -1;
    return base[index];
}

int specific_poke::get_iv(int index){
    if (index >= NUM_STATS || index < 0) return -1;
    return iv[index];
}

bool specific_poke::get_gender(){
    return gender;
}

bool specific_poke::is_shiny(){
    return shiny;
}

bool specific_poke::gen_moves(){
    int selected[2] = {-1,-1};
    bool found;
    // if (poke == NULL) return false;
    // pokemon_move potential[1000];
    // poke_move potential2[1000];
    std::vector<pokemon_move> potential;
    std::vector<poke_move> potential2;
    // int counter = 0;
    for (int i = 0; i < TOTAL_MOVES; i++){
        found = false;
        if (all_pokemon_moves[i].pokemon_id == species && all_pokemon_moves[i].pokemon_move_method_id == 1 && all_pokemon_moves[i].level <= level){
            for (int j = 0; j < (int) potential.size(); j++){
                if (find_move(all_pokemon_moves[i]).id == potential2.at(j).id) {
                    found = true;
                    break;
                }
            }
            if (!found){
                potential.push_back(all_pokemon_moves[i]);
                potential2.push_back(find_move(all_pokemon_moves[i]));
            }
        }
    }
    if (potential.empty()) return false;
    if (potential.size() == 1){
        move_stat_arr[0] = &potential.back();
        move_stat_arr[1] = null_stat;
    }
    else if (potential.size() == 2){
        move_stat_arr[0] = &potential.back();
        potential.pop_back();
        move_stat_arr[1] = &potential.back();
    }
    else{
        int decide;
        for(int i = 0; i < 2; i++){
            do {decide = rand()%potential.size();} while(selected[0] == decide || selected[1] == decide);
            selected[i] = decide;
            // else move_stat2 = &potential.at(decide);
            move_stat_arr[i] = &potential.at(decide);

        }
    }
    for(int i = 0; i < 2; i++){
        // pokemon_move *stat_var = move_stat1;
        // poke_move *move_var = move1;
        // if (i == 1){
        //     stat_var = move_stat2;
        //     move_var = move2;
        // } 
        // if(stat_var->move_id != -1) move_var = &find_move(*stat_var, move_types);
        if(move_stat_arr[i]->move_id != -1) move_arr[i] = &find_move(*move_stat_arr[i]);
        else move_arr[i] = null_move;
    }


    return true;
}

static bool same_type(specific_poke& p, poke_move& p_move){
    int index = p.id;
    int valid[2] = {-1,-1};
    if(p.id > 898) index -=9102;
        for (int i = index - 1; i < TOTAL_TYPES; i++){ //pokemon is guarenteed to be after entry index
            pokemon_type& curr = all_pokemon_types[i];
            if (p.id < curr.pokemon_id) continue;
            else if(p.id > curr.pokemon_id) return false;
            else if(p.id == curr.pokemon_id) {
                valid[0] = curr.type_id;
                if (i + 1 < TOTAL_TYPES && all_pokemon_types[i+1].pokemon_id == p.id) valid[1] = all_pokemon_types[i+1].type_id;
                break;
            }
        }
    if (valid[0] == p_move.type_id || valid[1] == p_move.type_id) return true;
    return false;
}

bool specific_poke::get_hit(specific_poke& attack, poke_move& p_move){
    double crit = 1.0;
    double stab = same_type(*this, p_move) ? 1.5 : 1.0;
    double type = 1.0;

    // if (move.type_id == poke.type)
    if (rand()%256 < attack.get_base(SPEED) / 2) crit = 1.5;
    double num = (((double)(2 * attack.get_level()) / 5) + 2) * p_move.power * ((double)attack.get_stat(ATT) / attack.get_stat(DEF));
    int dp = (num/50 + 2) * crit * ((rand()%16 + 85.0)/100.0) * stab * type;
    hp = MAX(0,hp - dp);
    if (crit == 1.5) return true;
    else return false;
}


bool specific_poke::operator==(specific_poke& p){
    for (int i = 0; i < 6; i++){
        if (this->get_base(i) != p.get_base(i) || this->get_stat(i) != p.get_stat(i)) return false;
    }
    return this->move_stat_arr == p.get_move_stat_arr() && this->move_arr == p.get_move_arr() && this->level == p.get_level() && 
    this->gender == p.get_gender() && this->shiny == p.is_shiny() && this->hp == p.get_hp() && this->pokemon::operator==(p);
}

bool specific_poke::operator!=(specific_poke& p){return !(*this == p);}


 //-------------------------------------------------------------------------------------------------------------------

std::string create(){
    std::ifstream dir;
    std::string prefix[3] = {(std::string)"/share/cs327/", (std::string)getenv("HOME") + (std::string)"/.poke327/",  (std::string)"/"};
    // std::string const_path = (std::string)"pokedex/pokedex/data/csv/" + name + (std::string)".csv";
    for(int i = 0; i < 3; i++){
        dir.open(prefix[i]/* + const_path*/);
        if(!dir.fail()) {
            // std::cout << "Opened database in \"" << prefix[i] << "\"!" << std::endl;
            return prefix[i];
        }
        else {
            // std::cout << "Cannot open database in \"" << prefix[i] << "\" trying new directory" << std::endl;
        } 
    }
    return "";
}
void* parse_file(std::string name){
    // char* files[6] = {(char*)"pokemon", (char*)"moves", (char*)"pokemon_moves", (char*)"pokemon_species", (char*)"expierence", (char*)"type_names"};
    std::string prefix = create();
    if (prefix.empty()) {
        // std::cout << "Cannot open the database" << std::endl;
        return NULL;
    }
    std::string file_name = "";
    if (!name.compare("pokemon") || !name.compare("moves") || !name.compare("pokemon_moves") || !name.compare("pokemon_species") || 
    !name.compare("experience") || !name.compare("type_names") || !name.compare("pokemon_stats") || !name.compare("pokemon_types")){
            file_name = name;
    }
    else return NULL;
    std::string const_path = (std::string)"pokedex/pokedex/data/csv/";
    
    std::ifstream file(prefix + const_path + file_name + (std::string)".csv");
    // file.open(prefix + const_path + files[0] + (std::string)".csv");
    if (file.fail()){
        // std::cout << "Cannot open the file \"" << file_name << ".csv\""<< std::endl;
        return NULL;
    }
    else {
        std::cout << "Opened \"" << file_name<< ".csv\"!"<< std::endl;
    }
    int rows = -1;
    std::string line;
    while (getline(file, line)){
        rows++;
    }
    if(file_name == "pokemon"){
       return make_pokemon(file, rows);
    }
    else if(file_name == "moves"){
       return make_moves(file, rows);
    }
    else if(file_name == "pokemon_moves"){
       return make_pokemon_moves(file, rows);
    }
    else if(file_name == "pokemon_species"){
       return make_pokemon_species(file, rows);
    }
    else if(file_name == "experience"){
       return make_experience(file, rows);
    }    
    else if(file_name == "type_names"){
       return make_type_name(file, rows);
    }
    else if(file_name == "pokemon_stats"){
       return make_stat(file, rows);
    }
    else if(file_name == "pokemon_types"){
       return make_pokemon_type(file, rows);
    }
    file.close();
    return NULL;
}

static int parse_int(std::string parser){
    if(parser != "") return stoi(parser);
    else return -1;
}

pokemon* make_pokemon(std::ifstream &file, int rows){
    pokemon *all_poke;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_poke = new pokemon[rows];    std::string id, species, height, weight, base, order, is_default, name; 
    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        getline(file, id, ',');
        getline(file, name, ',');
        getline(file, species, ',');
        getline(file, height, ',');
        getline(file, weight, ',');
        getline(file, base, ',');
        getline(file, order, ',');
        getline(file, is_default, '\n');

        all_poke[i].set_pokemon(parse_int(id), name, parse_int(species), parse_int(height), parse_int(weight), parse_int(base), parse_int(order),parse_int(is_default));
        //std::cout << all_poke[i];
    }
    return all_poke;
    // return true;
}

poke_move* make_moves(std::ifstream &file, int rows){
    poke_move *all_moves;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_moves = new poke_move[rows];    std::string id,identifier,generation_id,type_id,power,pp,accuracy,priority,
    target_id,damage_class_id,effect_id,effect_chance,contest_type_id,contest_effect_id,super_contest_effect_id; 

    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        getline(file, id, ',');
        getline(file, identifier, ',');
        getline(file, generation_id, ',');
        getline(file, type_id, ',');
        getline(file, power, ',');
        getline(file, pp, ',');
        getline(file, accuracy, ',');
        getline(file, priority, ',');
        getline(file, target_id, ',');
        getline(file, damage_class_id, ',');
        getline(file, effect_id, ',');
        getline(file, effect_chance, ',');
        getline(file, contest_type_id, ',');
        getline(file, contest_effect_id, ',');
        getline(file, super_contest_effect_id, '\n');


        all_moves[i].set_poke_move(parse_int(id), identifier, parse_int(generation_id), parse_int(type_id), parse_int(power), parse_int(pp), 
        parse_int(accuracy), parse_int(priority), parse_int(target_id), parse_int(damage_class_id), parse_int(effect_id), parse_int(effect_chance),
        parse_int(contest_type_id), parse_int(contest_effect_id), parse_int(super_contest_effect_id));
       // std::cout << all_moves[i];
    }
    // delete[] all_moves;
    return all_moves;
}

pokemon_move* make_pokemon_moves(std::ifstream &file, int rows){
    // printf("%ld, %d\n", sizeof(pokemon_move), rows);
    pokemon_move *all_pokemon_moves;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_pokemon_moves = new pokemon_move[rows];
    std::string pokemon_id,version_group_id,move_id,pokemon_move_method_id,level,order; 
    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        getline(file, pokemon_id, ',');
        getline(file, version_group_id, ',');
        getline(file, move_id, ',');
        getline(file, pokemon_move_method_id, ',');
        getline(file, level, ',');
        getline(file, order, '\n');

        all_pokemon_moves[i].set_pokemon_move(parse_int(pokemon_id), parse_int(version_group_id), parse_int(move_id), 
        parse_int(pokemon_move_method_id), parse_int(level), parse_int(order));
        //std::cout << all_pokemon_moves[i];
    }
    //  for(int i = 0; i < rows; i++){
    //    delete &all_pokemon_moves[i];
    // }
    return all_pokemon_moves;
    // return true;
}

pokemon_species* make_pokemon_species(std::ifstream &file, int rows){
    pokemon_species *all_species;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_species = new pokemon_species[rows];    

    std::string *fields = new std::string[20];



    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < 20; j++){
            char delim = ',';
            if(j == 19) delim = '\n';
            getline(file, fields[j], delim);
        }


        all_species[i].set_pokemon_species(parse_int(fields[0]), fields[1], parse_int(fields[2]), 
        parse_int(fields[3]), parse_int(fields[4]), parse_int(fields[5]), parse_int(fields[6]), parse_int(fields[7]), parse_int(fields[8]),
        parse_int(fields[9]), parse_int(fields[10]), parse_int(fields[11]), parse_int(fields[12]), parse_int(fields[13]), 
        parse_int(fields[14]), parse_int(fields[15]), parse_int(fields[16]), parse_int(fields[17]), parse_int(fields[18]), parse_int(fields[19]));
        //std::cout << all_species[i];
    }
    delete[] fields;
    return all_species;
    // return true;
}

experience* make_experience(std::ifstream &file, int rows){
    experience *all_exp;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_exp = new experience[rows];    

    std::string *fields = new std::string[3];



    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < 3; j++){
            char delim = ',';
            if(j == 2) delim = '\n';
            getline(file, fields[j], delim);
        }


        all_exp[i].set_experience(parse_int(fields[0]), parse_int(fields[1]), parse_int(fields[2]));
        //std::cout << all_exp[i];
    }
    delete[] fields;
    return all_exp;
    // return true;
}

type_name* make_type_name(std::ifstream &file, int rows){
    type_name *all_types;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_types = new type_name[rows];    

    std::string *fields = new std::string[3];



    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < 3; j++){
            char delim = ',';
            if(j == 2) delim = '\n';
            getline(file, fields[j], delim);
        }


        all_types[i].set_type_name(parse_int(fields[0]), parse_int(fields[1]), fields[2]);
        //std::cout << all_types[i];
    }
    delete[] fields;
    return all_types;
    // return true;
}

stat* make_stat(std::ifstream &file, int rows){
    stat *all_stat;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_stat = new stat[rows];    

    std::string *fields = new std::string[4];



    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < 4; j++){
            char delim = ',';
            if(j == 3) delim = '\n';
            getline(file, fields[j], delim);
        }


        all_stat[i].set_stat(parse_int(fields[0]), parse_int(fields[1]), parse_int(fields[2]), parse_int(fields[3]));
        //std::cout << all_stat[i];
    }
    delete[] fields;
    return all_stat;
    // return true;
}

pokemon_type* make_pokemon_type(std::ifstream &file, int rows){
    pokemon_type *all_types;/*(pokemon_move*)malloc(rows*sizeof(pokemon_move));*/ /**(new pokemon(1,"1",1,1,1,1,1,1));*/
    all_types = new pokemon_type[rows];    

    std::string *fields = new std::string[3];



    file.clear();
    file.seekg(0);
    std::string line;
    getline(file, line);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < 3; j++){
            char delim = ',';
            if(j == 2) delim = '\n';
            getline(file, fields[j], delim);
        }


        all_types[i].set_pokemon_type(parse_int(fields[0]), parse_int(fields[1]), parse_int(fields[2]));
        //std::cout << all_types[i];
    }
    delete[] fields;
    return all_types;
    // return true;
}







