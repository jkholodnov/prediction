#ifndef GAMES_H
#define GAMES_H

#include "game.h"
#include "team.h"
#include "Database.h"

#include <future>
#include <functional>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>

using namespace std;

class games_and_teams 
{
    public:
        games_and_teams();
        ~games_and_teams();
        void initialize_teams();
        void get_games();
        void parseGames();
    protected:
    private:
        vector<vector<game>> the_games{};
        //this is the only copy of teams//
        vector<team> the_teams{};
        Database* the_db;
};

#endif // GAMES_H
