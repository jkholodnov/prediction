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
    int generate_ELO();
    void generate_Performance_Rating();

    vector<vector<game>> the_games{};
    vector<team> the_teams{};
    vector<player> the_players{};
    Database* the_db;

   protected:
   private:
};

#endif  // GAMES_H
