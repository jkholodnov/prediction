#ifndef GAME_H
#define GAME_H

#include "team.h"
#include "player.h"
#include "RInside_Container.h"
#include "Database.h"
#include "dependencies.h"

using namespace std;

class game {
   public:
    game(const string& id, team* _team1, team* _team2, const int& team1Points,
         const int& team2Points);
    ~game();
    pair<int, string> generate_Team_ELO();
    vector<string> generate_performance_ratings(
        unordered_map<string, player>* the_players,
        shared_ptr<RInside_Container> RInside);
    vector<string> generate_player_PIR();

   protected:
   private:
    int team1Score{0};
    int team2Score{0};
    string gameid{};

    team* team1;
    team* team2;

    double team1Expected{0};
    double team2Expected{0};
};

#endif  // GAME_H
