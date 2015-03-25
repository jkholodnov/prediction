#ifndef GAME_H
#define GAME_H

#include "team.h"

#include <math.h>
#include <vector>
#include <string>

using namespace std;

class game
{
    public:
        game(const int& id, team* _team1, team* _team2,const int& team1Points, const int& team2Points);
        virtual ~game();
        int update_Team_Ratings();
    protected:
    private:
        int team1Score{0};
        int team2Score{0};
        int gameid{};

        team* team1;
        team* team2;

        double team1Expected{0};
        double team2Expected{0};
};

#endif // GAME_H
