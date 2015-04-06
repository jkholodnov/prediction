#ifndef PLAYER_H
#define PLAYER_H

#include "Database.h"
#include "dependencies.h"

using namespace std;

class player {
   public:
    player(string playname);
    ~player();
    void add_game_record(vector<double> data_to_add);
    pair<double, double> get_mean_and_stdevs();

   protected:
   private:
    string name{};
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
};

#endif  // PLAYER_H
