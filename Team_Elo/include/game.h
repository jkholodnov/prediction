#ifndef GAME_H
#define GAME_H

#include "team.h"
#include "Database.h"

#include <math.h>
#include <utility>
#include <vector>
#include <string>

using namespace std;

class game
{
    public:
        game(const string& id, team* _team1, team* _team2,const int& team1Points, const int& team2Points);
        ~game();
        pair<int,string> update_Team_Ratings();
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

#endif // GAME_H
