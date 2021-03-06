#include "../include/player.h"

player::player(const string &name, const string &teamname) {
    player_name = name;
    team_name = teamname;
}

player::~player() {
    // dtor
}

void player::get_player_scores(shared_ptr<RInside_Container> R_Inside_Container) {
    Database *predict_db = new Database("../predict.db");

    string _query =
        "SELECT a.minutes, a.fgm, a.fga, a.tpm, a.tpa, a.ftm, a.fta, a.oreb, "
        "a.dreb, a.assist, a.steal, a.block, a.turnover, a.fouls, a.plus_minus, "
        "a.points, b.day, a.gameID FROM gameData as a JOIN games as b ON "
        "a.gameID = b.gameID WHERE Name = '" +
        player_name + "' and a.injury = 'NULL' ORDER BY b.day;";
    auto All_Games = predict_db->query(_query);

#if TEST == 1
    if (All_Games.size() == 0) {
        cout << "Did not retrieve any game records from database for player: "
             << player_name << endl;
    }
#endif

    int games_counted{0};

    for (auto &Single_Game : All_Games) {
        games_counted++;
        // The player was NOT injured or scratched for this game.//
        minutes.emplace_back(atoi(Single_Game[0].c_str()));
        fga.emplace_back(atoi(Single_Game[1].c_str()));
        fgm.emplace_back(atoi(Single_Game[2].c_str()));

        tpa.emplace_back(atoi(Single_Game[3].c_str()));
        tpm.emplace_back(atoi(Single_Game[4].c_str()));

        fta.emplace_back(atoi(Single_Game[5].c_str()));
        ftm.emplace_back(atoi(Single_Game[6].c_str()));

        oreb.emplace_back(atoi(Single_Game[7].c_str()));
        dreb.emplace_back(atoi(Single_Game[8].c_str()));

        assist.emplace_back(atoi(Single_Game[9].c_str()));
        steal.emplace_back(atoi(Single_Game[10].c_str()));
        block.emplace_back(atoi(Single_Game[11].c_str()));
        turnover.emplace_back(atoi(Single_Game[12].c_str()));
        fouls.emplace_back(atoi(Single_Game[13].c_str()));

        plus_minus.emplace_back(atoi(Single_Game[14].c_str()));
        points.emplace_back(atoi(Single_Game[15].c_str()));
    }
    delete predict_db;
    /*SKIPPING FG, 3PT, FT FOR NOW COME BACK.//*/

    // initialize statistics objects for all data vectors.//

    if (games_counted != 0) {
        statistics minute_stats{minutes};
        statistics fga_stats{fga};
        statistics fgm_stats{fgm};

        statistics tpa_stats{tpa};
        statistics tpm_stats{tpm};

        statistics fta_stats{fta};
        statistics ftm_stats{ftm};

        statistics oreb_stats{oreb};
        statistics dreb_stats{dreb};

        statistics assist_stats{assist};
        statistics steal_stats{steal};
        statistics block_stats{block};
        statistics turnover_stats{turnover};
        statistics fouls_stats{fouls};
        statistics plus_minus_stats{plus_minus};
        statistics points_stats{points};

        // place statistics objects into std::unordered_map//
        mean_and_stdevs.emplace("minutes", minute_stats);
        mean_and_stdevs.emplace("fga", fga_stats);
        mean_and_stdevs.emplace("fgm", fgm_stats);

        mean_and_stdevs.emplace("tpa", tpa_stats);
        mean_and_stdevs.emplace("tpm", tpm_stats);

        mean_and_stdevs.emplace("fta", fta_stats);
        mean_and_stdevs.emplace("ftm", ftm_stats);

        mean_and_stdevs.emplace("oreb", oreb_stats);
        mean_and_stdevs.emplace("dreb", dreb_stats);

        mean_and_stdevs.emplace("assist", assist_stats);
        mean_and_stdevs.emplace("steal", steal_stats);
        mean_and_stdevs.emplace("block", block_stats);
        mean_and_stdevs.emplace("turnover", turnover_stats);
        mean_and_stdevs.emplace("fouls", fouls_stats);
        mean_and_stdevs.emplace("plus_minus", plus_minus_stats);
        mean_and_stdevs.emplace("points", points_stats);

        vector<thread> worker_threads;
        size_t i;
        for (i = 0; i < 100; i++) {
            game_simulations.emplace_back();
        }
        for (i = 0; i < 100; i++) {
            worker_threads.emplace_back(&simulation::simulate_players_performance,
                                        &game_simulations[i], mean_and_stdevs,
                                        keys_to_map, R_Inside_Container, player_name);
        }
        for (i = 0; i < worker_threads.size(); i++) {
            worker_threads[i].join();
        }
    }
}
int player::simulate_game_scores(int i) {
    // cout << "called" << endl;
    auto the_simulation = game_simulations[i];
    // cout << "called2" << endl;

    auto map_of_performances = the_simulation.simulated_performance;

    auto minutes = map_of_performances.find("minutes");
    auto fga = map_of_performances.find("fga");
    auto tpa = map_of_performances.find("tpa");
    auto fta = map_of_performances.find("fta");

    // auto fgm = map_of_performances.find("fgm");
    // auto tpm = map_of_performances.find("tpm");
    // auto ftm = map_of_performances.find("ftm");

    auto oreb = map_of_performances.find("oreb");
    auto dreb = map_of_performances.find("dreb");
    auto assist = map_of_performances.find("assist");
    auto steal = map_of_performances.find("steal");
    auto turnover = map_of_performances.find("turnover");
    auto fouls = map_of_performances.find("fouls");
    auto plus_minus = map_of_performances.find("plus_minus");

    double minutes_value = minutes->second;
    double fga_value = fga->second;
    double tpa_value = tpa->second;
    double fta_value = fta->second;

    // double fgm_value = fgm->second;
    // double tpm_value = tpm->second;
    // double ftm_value = ftm->second;

    double oreb_value = oreb->second;
    // double dreb_value = dreb->second;
    double assist_value = assist->second;
    double steal_value = steal->second;
    double turnover_value = turnover->second;
    double fouls_value = fouls->second;
    double plus_minus_value = plus_minus->second;

    // cout << minutes_value << "#" << fga_value << "#" << tpa_value << "#" <<
    // fta_value << "#" << oreb_value << "#" << assist_value << "#" << steal_value
    // << "#" << turnover_value << "#" << fouls_value << "#" << plus_minus_value
    // << endl;
    // These values are hard coded from my lm(Score ~ .) output. Need to figure
    // out a way to route RInside to here.
    double predicted_score = -0.520640 + 0.027873 * minutes_value + 0.959890 * fga_value +
                             0.206571 * tpa_value + 0.793915 * fta_value -
                             0.115459 * oreb_value - 0.153142 * assist_value -
                             0.077242 * steal_value + 0.107611 * turnover_value +
                             0.027402 * fouls_value + 0.087565 * plus_minus_value;
    if (predicted_score < 0) {
        predicted_score = 0;
    }

    return round(predicted_score);
}