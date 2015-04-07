#include "../include/player.h"

player::player(string playname) { name = playname; }

player::~player() {
    // dtor
}

void player::add_record(vector<string> data_to_add) {
#if TEST == 1
    if (data_to_add.size() != 17) {
        cout << "Not the correct number of data inputs." << endl;
    }
#endif

    minutes.emplace_back(atoi(data_to_add[1].c_str()));
    fgm.emplace_back(atoi(data_to_add[2].c_str()));
    fga.emplace_back(atoi(data_to_add[3].c_str()));
    tpm.emplace_back(atoi(data_to_add[4].c_str()));
    tpa.emplace_back(atoi(data_to_add[5].c_str()));
    ftm.emplace_back(atoi(data_to_add[6].c_str()));
    fta.emplace_back(atoi(data_to_add[7].c_str()));
    oreb.emplace_back(atoi(data_to_add[8].c_str()));
    dreb.emplace_back(atoi(data_to_add[9].c_str()));
    assist.emplace_back(atoi(data_to_add[10].c_str()));
    steal.emplace_back(atoi(data_to_add[11].c_str()));
    block.emplace_back(atoi(data_to_add[12].c_str()));
    turnover.emplace_back(atoi(data_to_add[13].c_str()));
    fouls.emplace_back(atoi(data_to_add[14].c_str()));
    plus_minus.emplace_back(atoi(data_to_add[15].c_str()));
    points.emplace_back(atoi(data_to_add[16].c_str()));
}

pair<double, double> player::get_mean_sd(const vector<double>& data_vector) {
    auto get_mean = [](const vector<double>& data) -> double {
        double mean{0.0};
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
