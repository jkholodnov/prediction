#ifndef TEAM_H
#define TEAM_H

#include "Database.h"

#include <fstream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

class team
{
    public:
        team(string abbr);
        ~team();
        void init(string abbr);
        double get_Rating();
        void update_Rating(double update_by);
        string getAbbr();
    protected:
    private:
        string team_Abbreviation{};
        int gamescore{};
        double bonus_Rating{0.0};
        double base_Rating{1500.0};
};


#endif // TEAM_Hŝ
