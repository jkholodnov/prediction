#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>
#include <RInside.h>

using namespace std;

class simulation
{
    public:
        simulation();
        virtual ~simulation();
    protected:
    private:
		double predicted_minutes;
        double predicted_fga;
        double predicted_fgm;
        double predicted_fg_percent;
        double predicted_tpa;
        double predicted_tpm;
        double predicted_tp_percent;
        double predicted_fta;
        double predicted_ftm;
        double predicted_ft_percent;
        double predicted_oreb;
        double predicted_dreb;
        double predicted_reb;
        double predicted_assist;
        double predicted_steal;
        double predicted_block;
        double predicted_turnover;
        double predicted_fouls;
        double predicted_plus_minus;
        double predicted_points;
        string predicted_injury_status;
};

#endif // SIMULATION_H
