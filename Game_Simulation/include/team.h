#ifndef TEAM_H
#define TEAM_H

#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <memory>

#include <RInside.h>
#include "Database.h"
#include "player.h"

using namespace std;

class team
{
    public:
        team(string teamname);
        virtual ~team();
        team(const team& other);
        team(team&& other);
        void generate_team_parallel(shared_ptr<RInside> R);
    protected:
    private:
        string team_name{""};
        vector<player> players;
        shared_ptr
};

#endif // TEAM_H
