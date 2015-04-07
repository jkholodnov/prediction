#ifndef TEAM_H
#define TEAM_H

#include "../include/dependencies.h"

#include "../include/RInside_Container.h"
#include "../include/Database.h"
#include "../include/player.h"

using namespace std;

class team {
   public:
    team(string teamname);
    virtual ~team();
    team(const team& other);
    team(team&& other);
    void generate_team_simulations(shared_ptr<RInside_Container> R_Inside_Container);
    vector<int> aggregate_player_scores();

    string team_name{""};
    vector<player> players;

   protected:
   private:
};

#endif  // TEAM_H
