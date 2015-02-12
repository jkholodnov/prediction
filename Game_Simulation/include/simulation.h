#ifndef SIMULATION_H
#define SIMULATION_H

#include <unordered_map>
#include <vector>
#include <thread>
#include <string>
#include <memory>
#include <iostream>
#include <utility>
#include <functional>


#include <RInside.h>
#include "../include/statistics.h"
#include "../include/RInside_Container.h"

using namespace std;

class simulation
{
    public:
        simulation();
        virtual ~simulation();
        void simulate_players_performance(const unordered_map<string, statistics>& mean_and_stdevs, const vector<string>& keys_to_map, shared_ptr<RInside_Container> R_Inside_Container);

    protected:
    private:
        unordered_map<string, double> simulated_performance{};
        string predicted_injury_status;
};

#endif // SIMULATION_H
