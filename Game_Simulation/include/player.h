#ifndef PLAYER_H
#define PLAYER_H

#include "../include/dependencies.h"

#include "../include/Database.h"
#include "../include/statistics.h"
#include "../include/simulation.h"

using namespace std;

class player
{
    public:
        player(const string& name, const string& teamname);
        virtual ~player();
        void get_player_scores(shared_ptr<RInside_Container> R_Inside_Container);
        int simulate_game_scores(int i);

        string team_name{""};
        string player_name{""};
        vector<double> minutes;
        vector<double> fga;
        vector<double> fgm;
        vector<double> tpa;
        vector<double> tpm;
        vector<double> fta;
        vector<double> ftm;
        vector<double> oreb;
        vector<double> dreb;
        vector<double> assist;
        vector<double> steal;
        vector<double> block;
        vector<double> turnover;
        vector<double> fouls;
        vector<double> plus_minus;
        vector<double> points;
        vector<string> injury_status;
        vector<string> keys_to_map{"minutes","fga","fgm","fg_percent_stats","tpa","tpm","tp_percent_stats","fta","ftm","ft_percent_stats","oreb","dreb","reb","assist","steal","block","turnover","fouls","plus_minus","points"};
        unordered_map<string, statistics> mean_and_stdevs{};

        volatile int simulation_count{0};
        vector<simulation> game_simulations{};
    protected:
    private:
};

#endif // PLAYER_H
