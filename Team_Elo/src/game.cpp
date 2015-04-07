#include "../include/game.h"

game::game(const string& id, team* _team1, team* _team2, const int& team1Points,
           const int& team2Points) {
    gameid = id;
    team1 = _team1;
    team2 = _team2;
    team1Score = team1Points;
    team2Score = team2Points;
}

game::~game() {}

// parallelize this call//
pair<int, string> game::generate_Team_ELO() {
    auto t1_Rating = team1->get_Rating();
    auto t2_Rating = team2->get_Rating();
    string _query = "UPDATE games SET team1ELO = " + to_string(t1_Rating) +
                    ", team2ELO = " + to_string(t2_Rating) + " WHERE gameid = '" +
                    gameid + "';";

    team1Expected = 1 / (1 + pow(10, ((t2_Rating - t1_Rating) / 400)));
    team2Expected = 1 / (1 + pow(10, ((t1_Rating - t2_Rating) / 400)));
    // Calculating Expected Win Ratio based on standard ELO formula.
    // cout << t1_Rating << "~" << team1Expected << "##" << t2_Rating << "~" <<
    // team2Expected << endl;
    if (team1Score > team2Score) {
        team1->update_Rating(50 * (1 - team1Expected));
        team2->update_Rating(50 * (0 - team2Expected));

        if (t1_Rating > t2_Rating) {
            // number_correct++;
            auto pair = make_pair(1, _query);
            return pair;
        } else {
            auto pair = make_pair(0, _query);
            return pair;
        }
    } else {
        team1->update_Rating(50 * (0 - team1Expected));
        team2->update_Rating(50 * (1 - team2Expected));

        if (t2_Rating > t1_Rating) {
            // number_correct++;
            auto pair = make_pair(1, _query);
            return pair;
        } else {
            auto pair = make_pair(0, _query);
            return pair;
        }
    }
}

vector<string> game::generate_performance_ratings(
    unordered_map<string, player>* the_players, shared_ptr<RInside_Container> RInside) {
    Database* the_db = new Database("../predict.db");
    vector<string> result_set{};
    auto players_gamedata = the_db->query(
        "SELECT "
        "Name,minutes,fgm,fga,tpm,tpa,ftm,fta,oreb,dreb,assist,steal,block,turnover,"
        "fouls,plus_minus,points FROM gamedata WHERE gameID = " +
        gameid + " and injury = 'NULL';");

    double game_performance{0.0};
    for (auto& single_game_data : players_gamedata) {
        player& current_player = the_players->at(single_game_data[0]);
        game_performance = 0.0;

        current_player.add_record(single_game_data);

        vector<pair<double, double>> mean_sd_pairs{};
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.minutes));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.fgm));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.fga));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.tpm));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.tpa));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.ftm));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.fta));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.oreb));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.dreb));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.assist));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.steal));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.block));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.turnover));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.fouls));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.plus_minus));
        mean_sd_pairs.emplace_back(current_player.get_mean_sd(current_player.points));

        vector<string> R_Queries{};

        for (unsigned i = 0; i < mean_sd_pairs.size(); i++) {
            string R_Query = "pnorm(" + single_game_data[i + 1] + ", mean = " +
                             to_string(mean_sd_pairs[i].first) + ", sd = " +
                             to_string(mean_sd_pairs[i].second) + ")";
            R_Queries.emplace_back(R_Query);
        }

        double variable_performance = R_Inside_Container->use(RInside_Query);
        game_performance += variable_performance;
        result_set.emplace_back("UPDATE gamedata SET performance_rating = " +
                                to_string(game_performance) + " WHERE gameID = " +
                                gameid + " AND Name = '" + players_gamedata[0] + "';");
    }

    delete the_db;
    return result_set;
}