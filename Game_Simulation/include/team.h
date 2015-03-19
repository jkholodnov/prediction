#ifndef TEAM_H
#define TEAM_H

#include "../include/dependencies.h"

#include "../include/RInside_Container.h"
#include "../include/Database.h"
#include "../include/player.h"

using namespace std;

class team
{
    public:
        team(string teamname);
        virtual ~team();
        team(const team& other);
        team(team&& other);
        void generate_team_simulations(shared_ptr<RInside_Container> R_Inside_Container);
        double aggregate_player_scores();
    protected:
    private:
        string team_name{""};
        vector<player> players;
};

#endif // TEAM_H
