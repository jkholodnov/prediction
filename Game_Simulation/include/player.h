#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <thread>
#include <functional>

#include "global_RInside.h"
#include "Database.h"
#include "statistics.h"
#include "simulation.h"

using namespace std;

class player
{
    public:
        player(const string& name);
        virtual ~player();
        void get_player_scores();
        void simulate_performance();
    protected:
    private:
        string player_name{};
        vector<double> minutes;
        vector<double> fga;
        vector<double> fgm;
        vector<double> fg_percent;
        vector<double> tpa;
        vector<double> tpm;
        vector<double> tp_percent;
        vector<double> fta;
        vector<double> ftm;
        vector<double> ft_percent;
        vector<double> oreb;
        vector<double> dreb;
        vector<double> reb;
        vector<double> assist;
        vector<double> steal;
        vector<double> block;
        vector<double> turnover;
        vector<double> fouls;
        vector<double> plus_minus;
        vector<double> points;
        vector<string> injury_status;
        unordered_map<string, statistics> mean_and_stdevs{};

        volatile int simulation_count{0};
        vector<simulation> game_simulations{};
};

#endif // PLAYER_H
