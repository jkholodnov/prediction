#ifndef TEAM_H
#define TEAM_H

#include "player.h"
#include "Database.h"
#include "dependencies.h"

using namespace std;

class team {
   public:
    team(string abbr);
    ~team();
    void init(string abbr);
    double get_Rating();
    void update_Rating(double update_by);
    string getAbbr();

    string team_Abbreviation{};
    int gamescore{};
    double bonus_Rating{0.0};
    double base_Rating{1500.0};
    vector<player> players_on_team{};

   protected:
   private:
};

#endif  // TEAM_H
