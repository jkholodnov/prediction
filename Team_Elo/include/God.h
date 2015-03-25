#ifndef GOD_H
#define GOD_H

#include "team.h"

#include <iostream>
#include <set>
#include <vector>
#include <string>

class God
{
    public:
        God();
        virtual ~God();
        void init_teams(std::set<std::string> all_games);
        team* get_Team_By_Abbr(std::string abbr);
    protected:
    private:
        std::vector<team> teams{};

};
#endif // GOD_H
