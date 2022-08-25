#include "user_interface.h"
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <math.h>

#define MAX_CHARS 16
#define ENTRANCE 8
#define HORIZ_LINE "-------------------------------------------------------------------------------------------------------------"
#define ANY_KEY "Press any key to continue"


static const int MOVE_X[9] = {0,0,-1,1,0,1,1,-1,-1}; //includes diagononals 
static const int MOVE_Y[9] = {-1,1,0,0,0,1,-1,-1,1};

static int letters = TRUE; //TRUE if Letters are being shown in print_controls, FALSE if numbers are
static char *trainers[HEIGHT * LENGTH]; //Array of all the trainers and their accompaning messages. See show_trainers

static int init_term(); //set up the screen for ncurses
static int print_controls(int);
int list_poke(character&, bool);

static bool more_colors;

bool battle(world*, specific_poke*, character*);

pokemon *all_pokemon;
poke_move *all_moves;
pokemon_move *all_pokemon_moves;
stat *all_stat;
pokemon_species *all_species;
experience *all_exp;
type_name *all_types;
pokemon_type* all_pokemon_types;

int absoluteValue(int a){ //find the absolute value of a, assume 0 is positive
  if(a < 0){
    return -1 * a;
  }
  return a;
}

static int color255(int color){
  return color * 200 / 51;
}

int make_color(bool more_colors){
  start_color();
  if (can_change_color()) { //making new colors and adjusting the old ones to make them look better 
    init_color(PLAYER, 1000, color255(125), 0); //orange
    init_color(COLOR_RED, 1000/*color255(125)*/, 0, 0/*1000*/);
    init_color(COLOR_YELLOW, 1000,1000,0);
    init_color(COLOR_MAGENTA, 1000,color255(51),1000);
    init_color(COLOR_CYAN, 0,1000,1000);
    init_color(COLOR_GREEN, 0,1000,0);
  }
  //printw("%d, TRUE: %d\n", can_change_color(), TRUE);
  init_pair(DEFAULT, COLOR_BLACK, COLOR_BLACK);
  init_pair(MARKET, COLOR_BLUE, COLOR_BLACK);
  init_pair(CENTER, COLOR_RED, COLOR_BLACK);
  init_pair(BOULDER, COLOR_WHITE, COLOR_BLACK);
  init_pair(PATH, COLOR_YELLOW, COLOR_BLACK);
  init_pair(CLEARING, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(GRASS, COLOR_GREEN, COLOR_BLACK);
  init_pair(TREE, COLOR_CYAN, COLOR_BLACK);
  if(more_colors){
    init_pair(PLAYER, PLAYER, COLOR_BLACK);
    init_pair(NPC, COLOR_RED, COLOR_BLACK);
  }


  return 0;
}

static bool set_name(std::string &name){
    move(5, 0);
    printw("                                                                   ");
    move(5, 0);
    mvprintw(6,0,"                                                                                      ");
    mvprintw(6,0, "Name cannot be longer than 20 characters");
    move(5,0);
    curs_set(1);
    nocbreak();
    echo();
    char temp[21];
    getstr(temp);
    // scanw("%s", temp);
    name = (std::string)temp;
    curs_set(0);
    raw();
    noecho();
    return true;
}

static bool print_stats(int x, int y, specific_poke& this_poke){
    // mvprintw(x, y, "You have encountered a %s", this_poke->name.data());
    if (this_poke.get_move(0).id == -1) mvprintw(y,x ,"No moves");
    else if ( this_poke.get_move(1).id == -1) mvprintw( y, x,"Move: %s", this_poke.get_move(0).identifier.data());
    else mvprintw(y, x,"Moves: %s, %s", this_poke.get_move(0).identifier.data(),  this_poke.get_move(1).identifier.data());
    mvprintw(y+1, x,"Level: %d", this_poke.get_level());
    mvprintw(y+2, x,"HP: %d", this_poke.get_stat(HP));
    mvprintw(y+3, x,"Attack: %d", this_poke.get_stat(ATT));
    mvprintw(y+4, x,"Defense: %d", this_poke.get_stat(DEF));
    mvprintw(y+5, x,"Special Attack: %d", this_poke.get_stat(SATT));
    mvprintw(y+6, x,"Special Defense: %d", this_poke.get_stat(SDEF));
    mvprintw(y+7, x,"Speed: %d", this_poke.get_stat(SPEED));
    char * gender = (char*)"female";
    if (this_poke.get_gender()) gender = (char*)"male";
    mvprintw(y+8, x,"Gender: %s", gender);
    char *shiny = (char*)"no";
    if (this_poke.is_shiny()) shiny = (char*)"yes";
    mvprintw(y+9, x,"Shiny?: %s", shiny);
    // mvprintw(y+10, x, HORIZ_LINE);
    return true;

}

bool start_game(world* planet){
    int ch;
    // attron(COLOR_PAIR(BOULDER));
    std::string pc_name;
    std::string r_name;
    erase();
    std::ifstream f;
    f.open("poke_script.csv");
    if (f.fail()) return false;
    std::string any_key = (std::string) ANY_KEY + (std::string)"\n";
    std::vector<char*> script;
    std::string line;
    while (std::getline(f, line, '$')){
        script.push_back(strdup(line.data()));
    }
    //script.push_back((char *)"testing");

    for (int i = 0; i < (int)script.size(); i++){
        int pos;
        line = (std::string) script.at(i);
        // printw("%s", script[i]);
        // refresh();
        if(!line.compare("$")) continue;
        while((pos = line.find("\r")) != (int)std::string::npos) {
            line.replace(pos, 1, (std::string)"");
        }
        if ((pos = line.find("\n")) == 0) line.replace(pos, 1, (std::string)"");
        while((pos = line.find("*pc*")) != (int)std::string::npos && pc_name.compare("")) {
            line.replace(pos, 4, pc_name);
        }
        while((pos = line.find("*r*")) != (int)std::string::npos && r_name.compare("")) {
            line.replace(pos, 3, r_name);
        }
        if ((pos = line.find("<input_pc>")) != (int)std::string::npos){
            set_name(pc_name);
            continue;
        }
        else if ((pos = line.find("<input_r>")) != (int)std::string::npos){
            set_name(r_name);
            continue;
        }
        erase();
        move(0,0);
        printw("%s", line.data());
        refresh();
        move(5,0) ;
        printw("%s", any_key.data());
        printw("To skip intro and use default names, unless already entered, press \'c\'");
        ch = getch();
        if (ch == 'c'){
            if (!pc_name.compare("")) pc_name = (std::string)"Red";
            if (!r_name.compare("")) r_name = (std::string)"Blue";
            break;
        }
    }
    for (int i = 0; i < (int)script.size(); i++){
        delete script[i];
    }


    character& pc = *(planet->mapn()->pc_cell()->person());
    specific_poke **starters = new specific_poke*[3];
    for (int i = 0; i < 3; i++){
        starters[i] = new specific_poke();
    }

    erase();
    // getch();



    for (int i = 0; i < 3; i++){
        specific_poke *temp = starters[i];
        int x = 0;
        if (i == 1) x = 41;
        if (i == 2) x = 81;
        mvprintw(2,x, "Press %d for: %s", i+1, temp->name.data());
        if (i == 0) mvprintw(3, 0, HORIZ_LINE);
        mvprintw(4, x, "%s", temp->name.data());
        print_stats(x,4,*temp);
        if (i==0) printw("\n%s", HORIZ_LINE);
    }
    // mvprintw(2,0, "Press 1 for:");
    // mvprintw(2,41, "Press 2 for:");
    // mvprintw(2,81, "Press 3 for:\n");
    // printw(HORIZ_LINE);
    mvprintw(0,0,"Here are your options, %s\n", pc_name.data());
    printw(HORIZ_LINE);
    for (int i = 2; i < 15; i++){
        mvprintw(i, 40, "|");
        mvprintw(i, 80, "|");
    }
    bool goAhead = false;
    do {
        ch = getch();
        if (ch < '4' && ch > '0') {
            pc.add_poke(*starters[ch - '1']);
            goAhead = true;
        }
        else mvprintw(15,0, "Enter either 1,2, or 3 to choose your Pokemon");
    }while(!goAhead);
	
	if(ch == '1') {
        delete starters[1];
        delete starters[2];
    }
	else if(ch == '2') {
        delete starters[0];
        delete starters[2];
    }
	else if(ch == '3') {
        delete starters[0];
        delete starters[1];
    }


    // attroff(COLOR_PAIR(BOULDER));
    delete[] starters;
    f.close();
    return true;
}


// bool add_trainer_poke(world* planet){
//     map* room = planet->mapn();
//     character* npc;
//     for (int j = 0; j < room->num_chars(); j++){
//         npc = room->get_char(j);
//         npc->add_poke(*(new specific_poke(all_pokemon, rand()%NUM_POKE, determine_level(planet), all_pokemon_moves, all_moves, all_stat)));
//         for (int i = 1; i < 6; i++){
//             int decide = rand()%10;
//             if (decide >= 6) break;
//             npc->add_poke(*(new specific_poke(all_pokemon, rand()%NUM_POKE, determine_level(planet), all_pokemon_moves, all_moves, all_stat)));
//         }
//     }
//     return true;
// }




//Note anytime there is printw and a bunch of spaces, that is to clear the line
int main(int argc, char *argv[]){
    all_pokemon = (pokemon*)parse_file((std::string)"pokemon");
    all_moves = (poke_move*)parse_file((std::string)"moves");
    all_pokemon_moves = (pokemon_move*)parse_file((std::string)"pokemon_moves");
    all_species = (pokemon_species*)parse_file((std::string)"pokemon_species");
    all_exp = (experience*)parse_file((std::string)"experience");
    all_types = (type_name*)parse_file((std::string)"type_names");
    all_stat = (stat*)parse_file((std::string)"pokemon_stats");
    all_pokemon_types = (pokemon_type*)parse_file((std::string)"pokemon_types");

    // for(int i = 0; i < TOTAL_MOVES; i++){
    //     if(all_pokemon_moves[i].move_id == 3 && all_pokemon_moves[i].level <= 1){
    //         std::cout << all_pokemon_moves[i];

    //     }
    // }
    // return 0;
    
    // return 0;
    int i, seed;
    seed = time(NULL);
    srand(seed);
    world *planet = new world();
    more_colors = true;
    init_term();
    //world_init(&planet);
    map *room = planet->mapn();
    int temp = rand()%MAX_CHARS;
    for (i = 0; i < argc; i++){
        if (!strcmp(argv[i], "--num-trainers") && argc >= i + 1){
            //room->set_num_chars( atoi(argv[i+1]));
            temp = atoi(argv[i+1]);
        }
        else if (!strcmp(argv[i], "--8-color")){
            more_colors = false;
        }
    }
    room->set_num_chars(temp);
    init_all_chars(room);
    // add_trainer_poke(planet);
    room->pc_cell()->person()->set_hn(heap_insert(room->char_heap(), room->pc_cell()->person())); //put the player into the heap for the first time
    // move(0,0);
    // printw("%d, %d", room->num_chars(), temp);
    make_color(more_colors);
    erase();
    start_game(planet);
    erase();
    move(1,0);
    room->print_map(more_colors);
    // move(HEIGHT + 1, 0);
    // printw("%d, %d", absoluteValue(planet->x() - 199), absoluteValue(planet->y() - 199));
    print_controls(letters);
    move_and_print_chars(planet);
    delete[] all_pokemon;
    delete[] all_moves;
    delete[] all_pokemon_moves;
    delete[] all_species;
    delete[] all_exp;
    delete[] all_types;
    delete[] all_stat;
    delete[] all_pokemon_types;
    return 0;
}

static int init_term(){
    initscr();
    raw(); 
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    return 0;
}

static int print_controls(int letters){
    move(1, LENGTH + 1);
    printw("Control Scheme: Letters");
    move(2,LENGTH + 1);
    printw("5 or \" \" or .:  Rest");
    move(3,LENGTH + 1);
    printw(">:              Enter building");
    move(4,LENGTH + 1);
    printw("<:              Exit building");
    move(5,LENGTH + 1);
    printw("t:              List Trainers");
    move(6,LENGTH + 1);
    printw("up and down:    navigate trainer list");
    move(7,LENGTH + 1);
    printw("esc:            leave trainer list");
    move(8,LENGTH + 1);
    printw("q or Q:         quit");

    if (!letters){
        move(1, LENGTH + 1);
        printw("Control Scheme: Numbers");
        move(HEIGHT - 12, LENGTH + 2);
        printw("7     8     9");
        move(HEIGHT - 11, LENGTH + 2);
        printw(" \\    |    / ");
        move(HEIGHT - 10, LENGTH + 2);
        printw("  \\   |   /  ");
        move(HEIGHT - 9, LENGTH + 2);
        printw("   \\  |  /   ");
        move(HEIGHT - 8, LENGTH + 2);
        printw("    \\ | /    ");
        move(HEIGHT - 7, LENGTH + 2);
        printw("     \\|/     ");
        move(HEIGHT - 6, LENGTH + 2);
        printw("4-----@-----6");
        move(HEIGHT - 5, LENGTH + 2);
        printw("     /|\\    ");
        move(HEIGHT - 4, LENGTH + 2);
        printw("    / | \\   ");
        move(HEIGHT - 3, LENGTH + 2);
        printw("   /  |  \\  ");
        move(HEIGHT - 2, LENGTH + 2);
        printw("  /   |   \\ ");
        move(HEIGHT - 1, LENGTH + 2);
        printw(" /    |    \\");
        move(HEIGHT, LENGTH + 2);
        printw("1     2     3");
        letters = FALSE;
    }
    else{
        move(1, LENGTH + 1);
        printw("Control Scheme: Letters");
        move(HEIGHT - 12, LENGTH + 2);
        printw("y     k     u");
        move(HEIGHT - 11, LENGTH + 2);
        printw(" \\    |    / ");
        move(HEIGHT - 10, LENGTH + 2);
        printw("  \\   |   /  ");
        move(HEIGHT - 9, LENGTH + 2);
        printw("   \\  |  /   ");
        move(HEIGHT - 8, LENGTH + 2);
        printw("    \\ | /    ");
        move(HEIGHT - 7, LENGTH + 2);
        printw("     \\|/     ");
        move(HEIGHT - 6, LENGTH + 2);
        printw("h-----@-----l");
        move(HEIGHT - 5, LENGTH + 2);
        printw("     /|\\    ");
        move(HEIGHT - 4, LENGTH + 2);
        printw("    / | \\   ");
        move(HEIGHT - 3, LENGTH + 2);
        printw("   /  |  \\  ");
        move(HEIGHT - 2, LENGTH + 2);
        printw("  /   |   \\ ");
        move(HEIGHT - 1, LENGTH + 2);
        printw(" /    |    \\");
        move(HEIGHT, LENGTH + 2);
        printw("b     j     n");
        letters = TRUE;
    }
    return 0;
}


int show_trainers(map *room){
    erase();
    move(0,0);
    int i, ch, height;
    character *npc;
    char *dir_x, *dir_y;
    char *defeat_state[2] = {(char*)"Not defeated", (char*)"Defeated"};
    char *types[7] = {(char*)"Hiker,        ", (char*)"Rival,        ", (char*)"Player,        ", (char*)"Pacer,        ", 
    (char*)"Wanderer,     ", (char*)"Stationary,   ", (char*)"Random Walker,"};
    trainers[0] = (char*)"Trainer List:\n";
    trainers[1] = (char*)"#:   Type:          Position relative to player:         State:\n";
    trainers[2] = (char*)"---------------------------------------------------------------------\n";
    for (i = 0; i < room->num_chars(); i++){
        npc = room->get_char(i);
        if (npc->type() != player){
            if (npc->x() > room->pc_x()) dir_x = (char*)"East";
            else dir_x = (char*)"West";
            if (npc->y() > room->pc_y()) dir_y = (char*)"South";
            else dir_y = (char*)"North";
            trainers[i+3] = (char *)malloc(200);
            snprintf(trainers[i + 3], 200, "%03d: %s %02d %s,   %02d %s,                 %s\n", i+1, types[npc->type()], absoluteValue(npc->y() - room->pc_y()),dir_y, 
            absoluteValue(npc->x() - room->pc_x()),dir_x, defeat_state[npc->def()]);
        }
    }
    trainers[room->num_chars() + 3] = (char*)"Press esc to leave trainer list\n";
    move(0,0);
    height = getmaxy(stdscr);
    for (i = 0; i < height; i++){
        if (i <= room->num_chars() + 3){
            printw("%s",trainers[i]);
        }
    }
    int top = 0;
    int bottom = height;
    if (height > room->num_chars() + 3) bottom = room->num_chars() + 4;
    do{
        ch = getch();
        if (ch == 27){
            erase();
            move(1,0);
            room->print_map(more_colors);
            print_controls(letters);
            refresh();
            break;
        }
        else if (ch == KEY_DOWN){
            if (room->num_chars()+4 < height || bottom >= room->num_chars() + 4) continue;
            top++;
            bottom++;
        }
        else if (ch == KEY_UP){
           if (top == 0) continue;
           top--;
           bottom--;
        }
        erase();
        move(0,0);
        for (i = top; i < bottom;i++){
            printw("%s", trainers[i]);
        }
    }while(ch != 27);
    i = 0;
    for (i = 3; i < room->num_chars() + 2; i++){
        if (trainers[i]) free(trainers[i]);
    }
    return 0;
}

bool health_bars(specific_poke* enemy, specific_poke* ally){
    char* hp_border = (char*)"**********************";
    if (enemy == NULL && ally == NULL) return false;
    if (enemy != NULL){
        mvprintw(0,0, "%s", hp_border);
        // move(1,0);
        // clrtoeol();
        mvprintw(1,0,"                             ");
        mvprintw(1,0, "*                    *");
        mvprintw(2,0, "%s", hp_border);
        // move(3,0);
        // clrtoeol();
        mvprintw(3,0,"                            ");
        mvprintw(3,0, "%s (%d / %d hp)",  enemy->name.data(), enemy->get_hp(), enemy->get_stat(HP));
        int health_bar = ((double)enemy->get_hp() / enemy->get_stat(HP)) * 20;
        for (int i = 0; i < health_bar; i++){
            mvprintw(1, 1 + i, "-");
        }
    }
    if (ally != NULL){
        move(18,0);
        clrtoeol();
        mvprintw(18,0, "%s", hp_border);
        mvprintw(19,0, "*                    *");
        mvprintw(20,0, "%s", hp_border);
        // move(21,0);
        // clrtoeol();
        mvprintw(21,0,"                                                             ");
        mvprintw(21,0, "%s (%d / %d hp)",  ally->name.data(), ally->get_hp(), ally->get_stat(HP));
        int health_bar = ((double)ally->get_hp() / ally->get_stat(HP)) * 20;
        for (int i = 0; i < health_bar; i++){
            mvprintw(19, 1 + i, "-");
        }
    }
    refresh();
    return true;
}

bool battle(world* planet, specific_poke* enemy, character* trainer){
    erase();
    if (enemy == NULL && trainer == NULL) return false;
    specific_poke* poke;
    if (trainer != NULL) {
        for (int i = 0; i < trainer->get_num_poke(); i++){
            char* def_arr[2] = {(char*)"Defeated", (char*)"Not Defeated"};
            move(i+1, 30);
            clrtoeol();
            mvprintw(i + 1, 30, "%d: %s --- %s", i+1, trainer->get_poke(i).name.data(), def_arr[(trainer->get_poke(i).get_hp() > 0)]);
        }
        if (trainer->get_active() == NULL)trainer->set_active(0);
        poke = trainer->get_active();
        mvprintw(0,0, "The trainer sent out a %s", poke->name.data());
    }
    else if (enemy != NULL) {
        poke = enemy; 
        mvprintw(0,0, "A wild %s appears", poke->name.data());
        // for (int i = 0; i < enemy->get_num_poke(); i++){
        //     char* def_arr[2] = {"Not defeated", "Defeated"};
        //     mvprintw(i + 1, 30, "%d: %s --- %s", i+1, enemy->get_poke(i).name.data(), def_arr[(enemy->get_poke(i).get_hp() > 0)]);
        // }
    }
    printw("\n%s", ANY_KEY);
    getch();
    character& pc = *(planet->mapn()->pc_cell()->person());
    mvprintw(0,0, "                                                          ");
    health_bars(poke, NULL);


    // mvprintw(23,0, "Choose a pokemon using the numbers listed!\n");
    // for (int i = 0; i < pc.get_num_poke(); i++){
    //     printw("%d. %s (%d hp)\n", i+1, pc.get_poke(i).name.data(), pc.get_poke(i).get_hp());
    // }
    // do{
    //     ch = getch();
    // }while(ch <= '0' || ch > pc.get_num_poke() + '0');
    int num;
    do{num = list_poke(pc, false);}while(num < 0);
    specific_poke& ally = pc.get_poke(num);
    move(23,0);
    clrtoeol();
    //erase();
    pc.set_active(num);
    health_bars(poke, &ally); 
    // mvprintw(23,0, "What do you want to do?\n1.Fight\n2.Change Pokemon\n3.Item\n4.Run");

   return true; 
}

// int damage(specific_poke& poke, poke_move& move){
//     double crit = 1.0;
//     double stab = 1.0;
//     double type = 1.0;
//     // if (move.type_id == poke.type)
//     if (rand()%256 < poke.get_base(SPEED) / 2) crit = 1.5;
//     double num = (((double)(2 * poke.get_level()) / 5) + 2) * move.power * ((double)poke.get_stat(ATT) / poke.get_stat(DEF));
//     return (num/50 + 2) * crit * (rand()%16 + 85) * stab * type;
// }

static int move_pri(poke_move& ally_m, poke_move& enemy_m, specific_poke& ally, specific_poke& enemy){
    if (ally_m.priority - enemy_m.priority) return ally_m.priority - enemy_m.priority;
    if (ally.get_stat(SPEED) - enemy.get_stat(SPEED)) return ally.get_stat(SPEED) - enemy.get_stat(SPEED);
    return (2 * rand()%2 - 1);
}

bool document_move(int index, bool successful, bool player, std::string move_name, std::string name){
    std::string message, user;
    if (player) user = "You";
    else user = "Trainer"; 
    switch(index){
        case FIGHT:
            if (successful)message = name + " used " + move_name + "! It hits!";
            else message = name + " used " + move_name +"! It misses.";
            break;
        case RUN:
            if (successful) message = "Successfully escaped the fight!";
            else message = "Attempt to run failed";
            break;
        case CHANGE:
            if (successful){
                if (player) message = name + " I choose you!";
                else message = user + " changed to " + name + "!";
            } 
            else message = "No more pokemon for " + user + "to switch to!";
            break;
        case REVIVE:
            if (successful) message = name + " revived!";
            else message = "Cannot revive " + name + ".";
            break;
        case POTION:
            if (successful) message = name + " healed 20 hp!";
            else message = "Could not heal " + name ;
            break;
        case BALL:
            if (successful) message = name + " was captured!";
            else message = "Could not capture " + name;
            break;
        default : 
            message = "default";
            break;
    }
    mvprintw(10,0, "                                                            ");
    // clrtoeol();
    mvprintw(10,0, message.data());
    printw("\n%s", ANY_KEY);
    refresh();
    getch();
    // move(10,0);
    // clrtoeol();
    mvprintw(10,0, "                                                            ");
    mvprintw(11,0, "                                                            ");
    // move(11,0);
    // clrtoeol();
    refresh();
    return true;
}

int list_poke(character& trainer, bool revive){
    // bool pc = (trainer->type() == player);
    bool stay = false;
    int ch;
    move(23,0);
    clrtoeol();
    mvprintw(23,0, "Choose a pokemon!");
    char* message = (char*)"";
    for (int i = 0; i < trainer.get_num_poke(); i++){
        message = (char*)"";
        if (&trainer.get_poke(i) == trainer.get_active()) message = (char*)"(CURRENT)";
        mvprintw(24+i, 0, "%d: %s, %d / %d hp %s", i+1, trainer.get_poke(i).name.data(), trainer.get_poke(i).get_hp(), trainer.get_poke(i).get_stat(HP), message);
    }
    do {
        do{ch = getch();}while((ch <= '0' || ch > trainer.get_num_poke() + '0') && ch != 'b');
        if (!revive && ch != 'b' && trainer.get_poke(ch - '1').get_hp() == 0) {
            mvprintw(23,0,"Pokemon must have more than 0 hp. Choose again.");
            ch = '0';
            stay = true;
            continue;
        }
        else if(revive && ch != 'b' && trainer.get_poke(ch - '1').get_hp() != 0){
            mvprintw(23,0,"Pokemon to revive must have 0hp. Choose again.");
            ch = '0';
            stay = true;
            continue;  
        }
        else if(ch != 'b'){
            stay = false;
        }
    } while(stay);
    if (ch == 'b'){
        return -1;
    }
    move(23,0);
    clrtobot();
    // return true;
    return ch - '1';
    // trainer.set_active(input);
    // document_move(CHANGE, true, pc, "", trainer.get_active()->name);
    // return true;
}

static bool can_revive(character& trainer){
    // mvprintw(10,0,"TESTING");
    // getch();
    for (int i = 0; i < trainer.get_num_poke(); i++){
        // printw("%d", trainer.get_poke(i) != *trainer.get_active());
        // getch();
        if (trainer.get_poke(i).get_hp() == 0 && trainer.get_poke(i) != *trainer.get_active()) return true;
    }
    return false;
}

int pc_choose_move(character& pc, int attempts, int speed, bool wild){
    do{
        specific_poke& active = *pc.get_active();
        mvprintw(23,0, "What do you want to do?\n1.Fight\n2.Item\n3.Run\n4.Change Pokemon");
        int ch = getch();
        int input = ch-'1';
        int odds;
        move(23,0);
        clrtobot();
        switch(input){
                case FIGHT:{
                    if (pc.get_active()->get_hp() == 0){
                        mvprintw(23,0,"Your pokemon has fainted! Change pokemon or revive it to continue\n%s", ANY_KEY);
                        continue;
                    }
                    mvprintw(23,0,"Choose a move:\n");
                    int bound = 2;
                    if (active.get_move(1).id == -1) {
                        printw("1. %s", pc.get_active()->get_move(0).identifier.data());
                        bound = 1;
                    }
                    else printw("1. %s\n2. %s", pc.get_active()->get_move(0).identifier.data(), pc.get_active()->get_move(1).identifier.data());
                    do {ch = getch();}while((ch <= '0' || ch > bound + '0') && ch != 'b');
                    if (ch=='b'){
                        move(23,0);
                        clrtobot();
                        continue;
                    }
                    else if(ch == '1'){
                        return MOVE1;
                    }
                    else if(ch == '2') return MOVE2;
                    // npc->get_active()->hit(damage(*pc.get_active(), pc.get_active()->get_move(ch-'1')));
                    // document_move(FIGHT, true, true, pc.get_active()->get_move(ch-'1').identifier, pc.get_active()->name);
                    // if (npc->get_active()->get_hp() == 0) done = true;
                    break;
                }
                case BAG:{
                    mvprintw(23,0,"Choose an item to use\n");
                    // while(true){
                    printw("1.Revive (%d left)\n2.Potion (%d left)\n3.Pokeball (%d left)\n", pc.get_item(REVIVE), pc.get_item(POTION), pc.get_item(BALL));
                    do {ch = getch();}while((ch <= '0' || ch > '3') && ch != 'b');
                    if (ch=='b'){
                        move(23,0);
                        clrtobot();
                        continue;
                    }
                    int input = ch - '1' + OFFSET;
                    if (pc.get_item(input) == 0){
                        mvprintw(10, 0,"You don't have anymore of that item to use!\n%s", ANY_KEY);
                        getch();
                        continue;
                    }
                    else if (input == REVIVE){
                        int val = -1;
                        if (can_revive(pc)){
                            move(23,0);
                            clrtobot();
                            val = list_poke(pc, true);
                            if (val == -1) continue;
                            return 20 + val;            
                        }
                        else {
                            mvprintw(10,0, "You have no pokemon to revive.\n%s", ANY_KEY);
                            getch();
                            mvprintw(10,0,"                                                     \n                                                ");
                            move(24,0);
                            clrtobot();
                            continue;
                        }
                    }
                    else if (input == BALL){
                        if (!wild){
                            mvprintw(10,0,"You cannot use a PokeBall in a battle with a trainer\n%s", ANY_KEY);
                            getch();
                            mvprintw(10,0,"                                                     \n                                                ");
                            continue;
                        }
                        else{
                            // if (pc.get_num_poke() >= 6){
                            //     mvprintw(10,0,"Your roster of pokemon is full!\n%s", ANY_KEY);
                            //     getch();
                            //     mvprintw(10,0,"                                                     \n                                                ");
                            //     continue;
                            } 
                            return BALL;
                    }
                    return input;
                        // else if (pc.get_item(ch-'1' + OFFSET)) return ch-'1' + OFFSET;
                    // }
                    // used = pc.use_item(ch - '1', false, NULL);
                    // if (used){
                    //     if (ch - '1' == REVIVE) mvprintw(0,23,"%s successfully revived to %d hp", pc.get_active()->name.data(), pc.get_active()->get_hp());
                    //     else if (ch - '1' == POTION) printw("%s successfully healed to %d hp", pc.get_active()->name.data(), pc.get_active()->get_hp());
                    //     // else if (ch - '1' == BALL) printw("%s successfully captured", pc.get_active()->name.data());
                    // }
                    // else if (!used && pc.get_item(ch-'1') != 0){
                    //     if (ch - '1' == REVIVE) printw("%s could not be revived", pc.get_active()->name.data(), pc.get_active()->get_hp());
                    //     else if (ch - '1' == POTION) printw("%s could not be healed", pc.get_active()->name.data(), pc.get_active()->get_hp());
                    //     else if (ch - '1' == BALL) printw("Cannot use a pokeball in a battle", pc.get_active()->name.data());
                    // }
                    // else printw("You have no more of this item to use");
                    // printw("\nPress any key to continue");
                    // getch();
                    break;
                }
                case RUN:{
                    attempts++;
                    odds = (int)floor((pc.get_active()->get_stat(SPEED) * 32)/((int)floor(speed / 4) % 256)) + 30*attempts;
                    if (rand()%256 < odds){
                        // document_move(RUN, true, true, "", ""); 
                        // mvprintw(23,0,"Attempt to flee successful!");
                        return RUN;
                    }
                    else return 0;
                    // getch();
                    // ch = 'q';
                    break;
                }
                case CHANGE:{
                    int val = list_poke(pc, false);
                    // bool succ = (val != -1);
                    if (val == -1) continue;
                    return 10 + val;
                    break;
                }
                default:
                    break;
        }
    }while(true);
    return -1;
}

bool message_at_0hp(character& pc){
    // bool no_poke = true;
    int val;
    // char* message;
    // char* arr[pc.get_num_poke()];
    // bool available[pc.get_num_poke()];
    if(pc.def()){
        mvprintw(10,0,"You have been defeated\n%s", ANY_KEY);
        return true;
    }
    val = list_poke(pc, false);
    pc.set_active(val);
    mvprintw(10,0,"%s, I choose you!\n%s", pc.get_active()->name.data(), ANY_KEY);
    getch();
    health_bars(NULL, pc.get_active());
    return false;

}

static int pc_action(int pc_move, character& pc, specific_poke& enemy, bool wild){
	if (pc_move == MOVE1 || pc_move == MOVE2){
		bool succ = pc.commit_move(FIGHT, enemy, wild, pc_move - MOVE1);
		document_move(FIGHT, succ, true, pc.get_active()->get_move(pc_move - MOVE1).identifier, pc.get_active()->name);
	}
	else if (pc_move == POTION){
		bool succ = pc.commit_move(pc_move, enemy /*this is a placeholder, not actually used*/, wild, 0);
		document_move(pc_move, succ, true, "", pc.get_active()->name);
	}
	else if (pc_move >= 20){
		specific_poke &active = *(pc.get_active());
		pc.set_active(pc_move - 20);
		pc.commit_move(REVIVE, *pc.get_active()/*placeholder, not actually used*/, wild, 0);
		document_move(REVIVE, true, true, "", pc.get_active()->name);
		pc.set_active(active);
	}
	else if (pc_move == RUN || pc_move == 0){
		//done = (bool) pc_move;
		//ran = (bool) pc_move;
		document_move(RUN, pc_move, true, "", "");
		if (pc_move) return false;
	}
	else if(pc_move >= 10){
		pc.set_active(pc_move - 10);
		document_move(CHANGE, true, true, "", pc.get_active()->name);
	}
    else if(pc_move == BALL){
        bool val = pc.use_item(BALL, true, &enemy);
        int ret = -2;
        // if (pc.get_num_poke() >= 6) val = false;
        if (val){
            // pc.add_poke(enemy);
            ret = -1;
        }
        document_move(pc_move, val, true, "", enemy.name);
        return ret;
    }
	return true;
}

bool encounter_pokemon(world *planet, pokemon *poke_arr){
    bool done, ran;
	map *room = planet->mapn();
	character& pc = *(room->pc_cell()->person());
    int chance, pc_1_move, attempts = 0;
    // pokemon poke = poke_arr[rand()%NUM_POKE];
    specific_poke *this_poke = new specific_poke(determine_level(planet->x(), planet->y()));
    // this_poke->gen_moves(all_pokemon_moves, all_moves);
    // this_poke->gen_stats(all_stat);
    //poke_move *moves = this_poke->get_moves();
    erase();
	battle(planet, this_poke, NULL);
	
    //printw("You have encountered a %s", this_poke->name.data());
    //print_stats(0,1,*this_poke);
    // if (this_poke->get_move(0).id == -1) printw("No moves");
    // else if ( this_poke->get_move(1).id == -1) printw("Move: %s\n", this_poke->get_move(0).identifier.data());
    // else printw("Moves: %s, %s\n", this_poke->get_move(0).identifier.data(),  this_poke->get_move(1).identifier.data());
    // printw("Level: %d\n", this_poke->get_level());
    // printw("HP: %d\n", this_poke->get_stat(HP));
    // printw("Attack: %d\n", this_poke->get_stat(ATT));
    // printw("Defense: %d\n", this_poke->get_stat(DEF));
    // printw("Special Attack: %d\n", this_poke->get_stat(SATT));
    // printw("Special Defense: %d\n", this_poke->get_stat(SDEF));
    // printw("Speed: %d\n", this_poke->get_stat(SPEED));
    // char * gender = (char*)"female";
    // if (this_poke->get_gender()) gender = (char*)"male";
    // printw("Gender: %s\n", gender);
    // char *shiny = (char*)"no";
    // if (this_poke->is_shiny()) shiny = (char*)"yes";
    // printw("Shiny?: %s\n", shiny);
    // printw("------------------------------------------------\n");
    // printw("After leveling up once\n");
    // this_poke->level_up();
    // printw("Level: %d\n", this_poke->get_level());
    // printw("HP: %d\n", this_poke->get_stat(HP));
    // printw("Attack: %d\n", this_poke->get_stat(ATT));
    // printw("Defense: %d\n", this_poke->get_stat(DEF));
    // printw("Special Attack: %d\n", this_poke->get_stat(SATT));
    // printw("Special Defense: %d\n", this_poke->get_stat(SDEF));
    // printw("Speed: %d\n", this_poke->get_stat(SPEED));
    /*printw("\npress esc to escape");
    refresh();
        do{
        ch = getch();
        if (ch == 27){
            erase();
            move(1,0);
            room->print_map(more_colors);
            // move(HEIGHT + 1, 0);
            // printw("%d, %d", absoluteValue(planet->x() - 199), absoluteValue(planet->y() - 199));
            print_controls(letters);
            refresh();
            break;
        }
    }while(ch != 27);*/
	
	do{
		pc_1_move = 1; //assume player goes first
		int pc_move = pc_choose_move(pc, attempts, this_poke->get_stat(SPEED), true);
		int move_choice;
		if (this_poke->get_move(1).id == -1) move_choice = 0;
		else move_choice = rand()%2;
		poke_move& npc_move = this_poke->get_move(move_choice);
		if (pc_move == MOVE1 || pc_move == MOVE2) pc_1_move = move_pri(pc.get_active()->get_move(pc_move - MOVE1), npc_move, *(pc.get_active()), *this_poke);
		
		if (pc_1_move < 0){
			chance = rand()%100;
			if (chance < npc_move.accuracy)pc.get_active()->get_hit(*this_poke, npc_move);
			document_move(FIGHT, chance < npc_move.accuracy, false, npc_move.identifier, this_poke->name);
			health_bars(this_poke, pc.get_active());
		}
		if (!pc.get_active()->get_hp()){
            bool def = message_at_0hp(pc);
            done = def;
            if(done) continue;
        }
		
        int act =  pc_action(pc_move, pc, *this_poke, true);

        ran = (act == 0);
        if (act == -1){
            // mvprintw(10,0,"You captured the pokemon");
            // getch();
            return true;
        }
        else if(act == -2){
            mvprintw(10,0,"The %s escaped\n%s", this_poke->name.data(), ANY_KEY);
            getch();
            done = true;
            ran = true;
            continue;
        }
		// ran = !pc_action(pc_move, pc, *this_poke, true);
		if (ran) {
			done = true;
			continue;
		}
        health_bars(this_poke, pc.get_active());
		
		if (pc_1_move > 0 && this_poke->get_hp()) {
			chance = rand()%100;
			if (chance < npc_move.accuracy)pc.get_active()->get_hit(*this_poke, npc_move);
			document_move(FIGHT, chance < npc_move.accuracy, false, npc_move.identifier, this_poke->name);
			health_bars(this_poke, pc.get_active());
        }
        if (!pc.get_active()->get_hp()){
            bool def = message_at_0hp(pc);
            done = def;
            if(done) continue;
        } 
        else if (!this_poke->get_hp()) done = true;
        health_bars(this_poke, pc.get_active());
        refresh();
		

	}while(!done);
	if (!ran) {
		if (!pc.def()) mvprintw(10, 0, "You defeated the %s!\n%s" , this_poke->name.data(), ANY_KEY);
		else mvprintw(10, 0, "You were defeated by the %s!\n%s" , this_poke->name.data(), ANY_KEY);
		// npc->set_def(true);
		getch();
	}
    delete this_poke;
    return true;

}

int fight_npc(world *planet, character *npc){
    int npc_move, choose = 0, pc_move;
    int attempts = 0;
    bool done = false;
    bool ran = false;
    int pc_1_move; //positive for pc moves 1st, negative if not
    // npc->set_def(true);
    char *types[7] = {(char*)"hiker", (char*)"rival", (char*)"player", (char*)"pacer", (char*)"wanderer",
     (char*)"stationary", (char*)"random walker"};
    // erase();
    // move(0,0);
    // printw("The %s was defeated!\n", types[npc->type()]);
    // printw("Press esc to continue");
    character &pc = *(planet->mapn()->pc_cell()->person());
    battle(planet, NULL, npc);
    do{
        pc_1_move = 1; //assume pc moves first
        npc_move = npc->choose_move();
        pc_move = pc_choose_move(pc, attempts, npc->get_active()->get_stat(SPEED), false);

        if((pc_move == MOVE1 || pc_move == MOVE2) && npc_move != FIGHT) pc_1_move = -1;
        else if((pc_move == MOVE1 || pc_move == MOVE2) && npc_move == FIGHT){
            choose = rand()%2;
            if (npc->get_active()->get_move(1).id == -1) choose = 0;
            pc_1_move = move_pri(pc.get_active()->get_move(pc_move - MOVE1), npc->get_active()->get_move(choose), *pc.get_active(), *npc->get_active());
        }
        
        if (pc_1_move < 0){
            bool succ = npc->commit_move(npc_move, *pc.get_active(), false, choose);
			//printw("%d", succ);
			//getch();
            document_move(npc_move, succ, false, npc->get_active()->get_move(choose).identifier, npc->get_active()->name);
            // else document_move(npc_move, true, false, "" , npc->get_active()->name);
        }
        health_bars(npc->get_active(), pc.get_active());
        // refresh();
		if (!pc.get_active()->get_hp()){
            bool def = message_at_0hp(pc);
            done = def;
            if(done) continue;
        }
		
		ran = !pc_action(pc_move, pc, *npc->get_active(), false);
		if (ran) {
			done = true;
			continue;
		}
        health_bars(npc->get_active(), pc.get_active());


        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        // if (input !=)
        
        if (pc_1_move > 0 && npc->get_active()->get_hp()) {
            bool succ = npc->commit_move(npc_move, *pc.get_active(), false, choose);
            // if (npc_move == FIGHT) document_move(npc_move, true, false, npc->get_active()->get_move(choose).identifier, npc->get_active()->name);
            document_move(npc_move, succ, false, npc->get_active()->get_move(choose).identifier , npc->get_active()->name);
        }
        if (!pc.get_active()->get_hp()){
            bool def = message_at_0hp(pc);
            done = def;
            if(done) continue;
        } 
        else if (npc->def()) done = true;
        health_bars(npc->get_active(), pc.get_active());
        refresh();
        for (int i = 0; i < npc->get_num_poke(); i++){
            char* def_arr[2] = {(char*)"Not defeated", (char*)"Defeated"};
            move(i+1, 30);
            clrtoeol();
            mvprintw(i + 1, 30, "%d: %s --- %s", i+1, npc->get_poke(i).name.data(), def_arr[(int)(npc->get_poke(i).get_hp() == 0)]);
        }
    }while(!done);
    if (!ran) {
        if (!pc.def()) mvprintw(10, 0, "You defeated the %s!\n%s" , types[npc->type()], ANY_KEY);
        else mvprintw(10, 0, "You were defeated by the %s!\n%s" , types[npc->type()], ANY_KEY);
        // npc->set_def(true);
        getch();
    }
    // else mvprintw(10, 0, "You successfully fleed the fight\nPress any key to continue");
    // getch();
    erase();
    return 0;

}

int enter_building(map *room){
    int ch;
    erase();
    move(0,0);
    terrain_t building = room->cell(room->pc_x(), room->pc_y())->terr();
    char *message;
    if(building == market) message = (char*)"PokeMart";
    else if(building == center) message = (char*)"PokeCenter";
    else message = (char*)"???";
    printw("You have entered a %s\n", message);
    printw("press < to exit");
    do{
        ch = getch();
        if (ch == '<'){
            erase();
            move(1,0);
            room->print_map(more_colors);
            // move(HEIGHT, 0);
            // printw("%d, %d", planet->x(), planet->y());
            print_controls(letters);
            refresh();
            break;
        }
    }while(ch != '<');
    return 0;
}

int move_and_print_chars(world* planet){
    map_cell *old;
    character *npc;
    map *room = planet->mapn();

    while ((npc = (character*)heap_remove_min(room->char_heap()))){
        room = planet->mapn(); //update which room we're in
        refresh();
        int ch;
        if (npc->type() == player) {
            ch = getch(); 
            if (perform_instr(ch, planet)){ //if ch == 'q', perform_instr returns 1 so quit game
                endwin();
                delete planet;
                return 0;
            }
            room = planet->mapn(); //if the player changed rooms, update that
            if(!room->char_heap()) {
                room->set_char_heap((heap_t*)malloc(sizeof(heap_t)));
                heap_init(room->char_heap(), compare_char_time, NULL);
            }
			refresh();
            calculate_new_paths(room);
            npc->set_hn(heap_insert(room->char_heap(), npc)); //put player back in heap
            continue;
        }
        if (npc->def()){ //if an npc was defeated, just add them back into the heap with the time of their current cell added
            npc->add_time(room->cell(npc->x(), npc->y())->cost(npc->type()));
            npc->set_hn(heap_insert(room->char_heap(), npc));
            continue;
        }
        old = room->cell(npc->x(), npc->y());
        if(move_char(room, npc) == 2){ //move_char returns 2 if an npc goes into a player
            if (!room->pc_cell()->person()->def()){
                fight_npc(planet, npc);
                move(1,0);
                room->print_map(more_colors);
                print_controls(letters);
                refresh();
            }
            else mvprintw(0,0,"You have been defeated and cannot fight any trainers");
        }
        npc->set_hn(heap_insert(room->char_heap(), npc));
        move(old->y() + 1, old->x());
        old->print_map_cell(more_colors);
        move(npc->y() + 1, npc->x());
        room->cell(npc->x(), npc->y())->print_map_cell(more_colors);
        refresh();
    }
    return 1;
}



int perform_instr(int instr, world *planet){
    int moved = TRUE;
    direction_t dir = none;
    switch(instr){
        case '7': //up_l
        case 'y':
            dir = north_w;
            break;
        case '8': //up
        case 'k':
            dir = north;
            break;
        case '9': //up_r
        case 'u':
            dir = north_e;
            break;
        case '6'://right
        case 'l':
            dir = east;
            break;
        case '3': //down_r
        case 'n':
            dir = south_e;
            break;
        case '2': //down
        case 'j':
            dir = south;
            break;
        case '1'://down_l
        case 'b':
            dir = south_w;
            break;
        case '4'://left
        case 'h':
            dir = west;
            break;
        case '>':{ ; //enter
            map *current = planet->mapn();
            terrain_t terrain = current->cell(current->pc_x(), current->pc_y())->terr();
            if (terrain == market || terrain == center) enter_building(current);
            else {
                move(0, 0);
                printw("                                                                       ");
                move(0, 0);
                printw("You must be standing on a building to enter it");
            }
            moved = 0;
            break;
        }
        case '<': {//will never happen when in a building because that is taken care of in enter_building
            move(0, 0);
            printw("                                                                       ");
            move(0, 0);
            printw("You must have already entered a building to exit");
            moved = 0;
            break;
        }
        case ' ': //rest
        case '5':
        case '.':{ ;
            map *room = planet->mapn();
            room->cell(room->pc_x(), room->pc_y())->person()->add_time(room->cell(room->pc_x(), room->pc_y())->cost(player));
            moved = 0;
            break;
        }
        case 't':{ //display trainer stats
            show_trainers(planet->mapn());
            moved = 0;
            break;
        }
        case KEY_UP: //up
        case KEY_DOWN:{ //down
            move(0, 0);
            printw("                                                                       ");
            move(0, 0);
            printw("Press t in to open the trainers list in order to navigate it");
            moved = 0;
            break;
        }
        case '=':{
            if (letters) letters = FALSE;
            else letters = TRUE;
            print_controls(letters);
            moved = 0;
            break;
        }
        case 27:{ //escape
            move(0, 0);
            printw("                                                                       ");
            move(0, 0);
            printw("You must be in a menu to leave the menu");
            moved = 0;
            break;
        }
        case 'Q':
        case 'q': //quit
            return 1;
        default:{
            move(0, 0);
            printw("                                                                       ");
            move(0, 0);
            printw("Please enter a valid command");
            return 0;
            break;
        }
    }
    if (moved){
        move(0,0);
        printw("                                                                                                ");
        move_player(planet, dir);
    }
    return 0;
}

static int can_move(int x, int y, map *room){
    if (x >= LENGTH || x < 0 || y >= HEIGHT || y < 0 ) return 0;
    return 1;
}

int move_player(world *planet, direction_t dir){
    int newX, newY;
    map *current_map = planet->mapn();
    character *pc = planet->mapn()->pc_cell()->person();
    if (!can_move(current_map->pc_x() + MOVE_X[dir], current_map->pc_y() + MOVE_Y[dir], current_map)) return 1;
    map_cell *next = current_map->cell(current_map->pc_x() + MOVE_X[dir], current_map->pc_y() + MOVE_Y[dir]);
    if(next->person() && !next->person()->def() && !pc->def()){ //fight an npc in the way if they are not defeated
        // pc->def();
        fight_npc(planet, next->person());
        return 1;
    }
    else if (next->person() && pc->def()){
        mvprintw(0,0,"You have been defeated and cannot fight any trainers");
        return 1;
    }
    else if(next->person() && next->person()->def()){ //message that the enemy was already defeated
        char *types[7] = {(char*)"hiker", (char*)"rival", (char*)"player", (char*)"pacer", (char*)"wanderer", (char*)"stationary", (char*)"random walker"};
        char *trainer = types[next->person()->type()];
        move(0, 0);
        printw("                                                         ");
        move(0, 0);
        printw("You have already defeated this %s!", trainer);
        return 1;
    }
    else if (next->cost(player) >= INT_MAX) { //message that something is in the way
        terrain_t terr = next->terr();
        char *terr_message;
        switch(terr){
            case(boulder):
            case(bound):
                terr_message =(char*) "boulder";
                break;
            case (tree):
            case (forest):
                terr_message =(char*) "tree";
            break;
            default:
                terr_message = (char*)"???";
        }
        move(0, 0);
        printw("                                                         ");
        move(0, 0);
        printw("A %s blocks your way!", terr_message);

        return 1;
    }
    else if(next->terr() == (terrain_t)ENTRANCE){
        pc = current_map->pc_cell()->person();
        direction_t dir_code;
        if (current_map->entrance(north) == next->x() && next->y() == 0){
            dir_code = north;
            newX = current_map->entrance(north);
            newY = HEIGHT - 1;
        }
        else if (current_map->entrance(south) == next->x() && next->y() == HEIGHT - 1){
            dir_code = south;
            newX = current_map->entrance(south);
            newY = 0;
        }
        else if (current_map->entrance(west) == next->y() && next->x() == 0){
            dir_code = west;
            newX = LENGTH - 1;
            newY = current_map->entrance(west);
        }
        else if (current_map->entrance(east) == next->y() && next->x() == LENGTH - 1){
            dir_code = east;
            newX = 0;
            newY = current_map->entrance(east);
        }
        else return 1;
        planet->move_map(dir_code);
        reset_char_times(current_map); //reset all char times in prev map to 0 and make it so they are not defeated
        pc->set_time(0);

        current_map->pc_cell()->set_person(NULL); //remove the player from the previous map
        current_map->set_pc(-1, -1);

        current_map = planet->mapn(); //go to the new map
        current_map->set_pc(newX, newY);
        pc->set_x(newX);
        pc->set_y(newY);
        current_map->pc_cell()->set_person(pc);
        if (!planet->mapn()->all_chars()){ //if the planet is new and no chars have been added so add them
            current_map->set_num_chars( (rand()%MAX_CHARS));
            init_all_chars(current_map);
            // add_trainer_poke(planet);
        }
        move(1,0);
        current_map->print_map(more_colors);
        // move(HEIGHT + 1, 0);
        // printw("%d, %d", absoluteValue(planet->x() - 199), absoluteValue(planet->y() - 199));
        print_controls(letters);
        refresh();
        return 0;
    }
    pc = current_map->pc_cell()->person();
    current_map->pc_cell()->set_person(NULL);
    move(current_map->pc_y() + 1, current_map->pc_x());
    current_map->pc_cell()->print_map_cell(more_colors);
    refresh();
    newX = current_map->pc_x() + MOVE_X[dir];
    newY = current_map->pc_y() + MOVE_Y[dir];
    current_map->set_pc(newX, newY);
    pc->set_x(current_map->pc_x());
    pc->set_y(current_map->pc_y());
    current_map->pc_cell()->set_person(pc);
    pc->add_time(current_map->pc_cell()->cost(player));
    move(current_map->pc_y() + 1, current_map->pc_x());
    current_map->pc_cell()->print_map_cell(more_colors);
    refresh();
    int poke = rand()%10;
    if (!poke && !pc->def() && (current_map->pc_cell()->terr() == tall_grass0 || current_map->pc_cell()->terr() == tall_grass1)){
		encounter_pokemon(planet, all_pokemon);
		erase();
		move(1,0);
		current_map->print_map(more_colors);
		print_controls(letters);
		refresh();
	}
    return 0;
}
