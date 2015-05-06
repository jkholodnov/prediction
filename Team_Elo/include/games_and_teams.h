#ifndef GAMES_H
#define GAMES_H

#include "team.h"
#include "game.h"
#include "RInside_Container.h"
#include "Database.h"
#include "dependencies.h"

using namespace std;

class games_and_teams {
   public:
    games_and_teams();
    ~games_and_teams();
    void initialize_teams();
    void load_in_games();

    int compute_ELO();
    void compute_NPR();
    void compute_PIR();
    void generate_Performance_Rating();

    unordered_map<string, player> the_players;
    unordered_map<string, game> games_map;
    vector<vector<string>> the_games{};
    vector<team> the_teams{};
    Database* the_db;

   protected:
   private:
};

#endif  // GAMES_H
