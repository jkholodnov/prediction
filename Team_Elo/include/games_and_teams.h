#ifndef GAMES_H
#define GAMES_H

#include "game.h"
#include "team.h"

#include <vector>
#include <set>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std;

class games_and_teams 
{
    public:
        games();
        virtual ~games();
        void add_game();
        void get_games();
        void parseGames();
    protected:
    private:
        vector<game> the_games{};
        //this is the only copy of teams//
        vector<team> the_teams{};
};

#endif // GAMES_H
