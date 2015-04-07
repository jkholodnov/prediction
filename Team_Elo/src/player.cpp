#include "../include/player.h"

player::player(string playname) { name = playname; }

player::~player() {
    // dtor
}

double player::add_record(vector<string> data_to_add) {
#if TEST == 1
    if (data_to_add.size() != 17) {
        cout << "Not the correct number of data inputs." << endl;
    }
#endif

    minutes.emplace_back(data_to_add[1]);
    fgm.emplace_back(data_to_add[2]);
    fga.emplace_back(data_to_add[3]);
    tpm.emplace_back(data_to_add[4]);
    tpa.emplace_back(data_to_add[5]);
    ftm.emplace_back(data_to_add[6]);
    fta.emplace_back(data_to_add[7]);
    oreb.emplace_back(data_to_add[8]);
    dreb.emplace_back(data_to_add[9]);
    assist.emplace_back(data_to_add[10]);
    steal.emplace_back(data_to_add[11]);
    block.emplace_back(data_to_add[12]);
    turnover.emplace_back(data_to_add[13]);
    fouls.emplace_back(data_to_add[14]);
    plus_minus.emplace_back(data_to_add[15]);
    points.emplace_back(data_to_add[16]);
}

pair<double, double> player::get_mean_sd(const vector<double>& data_vector) {
    auto get_mean = [](const vector<double>& data) -> double {
        for (auto& i : data) {
            mean += i;
        }
        mean /= data.size();
        return mean;
    };

    double mean = get_mean(data_vector);

    auto get_stdev = [](double mean, const vector<double>& data) {
        double value_minus_mean{0.0};
        vector<double> residuals{};
        for (auto& i : data_vector) {
            value_minus_mean = 0.0;
            value_minus_mean = i - mean;
            residuals.emplace_back(pow(value_minus_mean, 2));
        }
        double stdev_squared = get_mean(residuals);
        return sqrt(stdev_squared);
    };

    double stdev = get_stdev(mean, data_vector);

    return make_pair(mean, stdev);
}
