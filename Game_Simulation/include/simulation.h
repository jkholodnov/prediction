#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>
#include <RInside.h>
#include <memory>
#include <unordered_map>


#include "../include/statistics.h"
#include "../include/RInside_Container.h"

using namespace std;

class simulation
{
    public:
        simulation(const unordered_map<string, statistics>& mean_and_stdevs, shared_ptr<RInside_Container> R_Inside_Container, const vector<string>& data_variables);
        virtual ~simulation();
    protected:
    private:
        unordered_map<string, double> simulated_performance{};
        string predicted_injury_status;
};

#endif // SIMULATION_H
