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

pair<int, string> game::generate_Team_ELO() {
    /**
     * @brief Determine the team's previous elo, account for that game's performance, then
     * look at this game's results.
     */
    Database* the_db = new Database("../2015.db");
    if (team1->bonus_Rating == 0) {
        cout << "TRUE1" << endl;
        // This is the first time the team has played a game, or we are starting to update
        // elo mid-season.
        string elo_counts = "SELECT COUNT(day) FROM games WHERE (team1abbr = '" +
                            team1->team_Abbreviation + "' OR team2abbr = '" +
                            team1->team_Abbreviation + "') and team1elo != 'NULL';";

        auto counts = the_db->query(elo_counts);
        if (counts[0][0] == "0") {
            // There are no games where Elo was developed.
        } else {
            // We are starting to analyze midway through the season. Get the team's
            // previous Elo, adjust for that game's results, update db.
            string elo_query =
                "SELECT team1abbr, team2abbr, team1elo, team2elo, "
                "team1score, team2score, MAX(day) FROM games WHERE (team1abbr = '" +
                team1->team_Abbreviation + "' OR team2abbr = '" +
                team1->team_Abbreviation + "') and team1elo != 'NULL';";

            auto last_game = the_db->query(elo_query);

            if (last_game[0][0] == team1->team_Abbreviation) {
                // team1 in our query is the team we need

                auto current_team = stod(last_game[0][2]);
                auto other_team = stod(last_game[0][3]);

                team1Expected = 1 / (1 + pow(10, ((other_team - current_team) / 400)));

                if (last_game[0][4] > last_game[0][5]) {
                    current_team += (50 * (1 - team1Expected));
                } else {
                    current_team += (50 * (0 - team1Expected));
                }

                auto bonus_elo = current_team - 1500.00;
                team1->bonus_Rating = bonus_elo;

            } else {
                // team2 in our query is the team we need

                auto current_team = stod(last_game[0][3]);
                auto other_team = stod(last_game[0][2]);

                team1Expected = 1 / (1 + pow(10, ((other_team - current_team) / 400)));

                if (last_game[0][4] < last_game[0][5]) {
                    current_team += (50 * (1 - team1Expected));
                } else {
                    current_team += (50 * (0 - team1Expected));
                }

                auto bonus_elo = current_team - 1500.00;
                team1->bonus_Rating = bonus_elo;
            }
        }
    }

    if (team2->bonus_Rating == 0) {
        cout << "TRUE2" << endl;
        // This is the first time the team has played a game, or we are starting to update
        // elo mid-season.
        // This is the first time the team has played a game, or we are starting to update
        // elo mid-season.
        string elo_counts = "SELECT COUNT(day) FROM games WHERE (team1abbr = '" +
                            team2->team_Abbreviation + "' OR team2abbr = '" +
                            team2->team_Abbreviation + "') and team1elo != 'NULL';";

        auto counts = the_db->query(elo_counts);
        if (counts[0][0] == "0") {
            // There are no games where Elo was developed.
        } else {
            // We are starting to analyze midway through the season. Get the team's
            // previous Elo, adjust for that game's results, update db.
            string elo_query =
                "SELECT team1abbr, team2abbr, team1elo, team2elo, "
                "team1score, team2score, MAX(day) FROM games WHERE (team1abbr = '" +
                team2->team_Abbreviation + "' OR team2abbr = '" +
                team2->team_Abbreviation + "') and team1elo != 'NULL';";

            auto last_game = the_db->query(elo_query);

            if (last_game[0][0] == team2->team_Abbreviation) {
                // team1 in our query is the team we need
                auto current_team = stod(last_game[0][2]);
                auto other_team = stod(last_game[0][3]);

                team1Expected = 1 / (1 + pow(10, ((other_team - current_team) / 400)));

                if (last_game[0][4] > last_game[0][5]) {
                    current_team += (50 * (1 - team1Expected));
                } else {
                    current_team += (50 * (0 - team1Expected));
                }

                auto bonus_elo = current_team - 1500.00;
                team1->bonus_Rating = bonus_elo;

            } else {
                // team2 in our query is the team we need

                auto current_team = stod(last_game[0][3]);
                auto other_team = stod(last_game[0][2]);

                team1Expected = 1 / (1 + pow(10, ((other_team - current_team) / 400)));

                if (last_game[0][4] < last_game[0][5]) {
                    current_team += (50 * (1 - team1Expected));
                } else {
                    current_team += (50 * (0 - team1Expected));
                }

                auto bonus_elo = current_team - 1500.00;
                team2->bonus_Rating = bonus_elo;
            }
        }
    }
    delete the_db;

    cout << team1->bonus_Rating << endl;
    cout << team2->bonus_Rating << endl;
    /**
     * @brief We now have the Elo rating of each team correctly updated in memory.
     */
    auto t1_Rating = team1->get_Rating();
    auto t2_Rating = team2->get_Rating();

    string _query = "UPDATE games SET team1ELO = " + to_string(t1_Rating) +
                    ", team2ELO = " + to_string(t2_Rating) + " WHERE gameid = '" +
                    gameid + "';";

    team1Expected = 1 / (1 + pow(10, ((t2_Rating - t1_Rating) / 400)));
    team2Expected = 1 / (1 + pow(10, ((t1_Rating - t2_Rating) / 400)));

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

vector<string> game::generate_NPR(unordered_map<string, player>* the_players,
                                  shared_ptr<RInside_Container> RInside) {
    Database* the_db = new Database("../2015.db");
    vector<string> result_set{};

    string query =
        "SELECT Name, minutes,fgm, fga, tpm, tpa, ftm, fta, oreb, dreb, assist, steal, "
        "block, turnover, fouls, plus_minus, points FROM gamedata WHERE gameID "
        "= " +
        gameid + " and injury = 'NULL';";

    /**
     * output .'s to demonstrate that the function hasn't halted.
     */
    if (atoi(gameid.c_str()) % 3 == 0) {
        cout << "." << flush;
    }

    auto players_gamedata = the_db->query(query);

#if TEST == 1
    if (players_gamedata.size() == 0) {
        cout << "Did not retrieve any gamedata for game: " << gameid << endl;
    }
#endif

    double game_performance{0.0};
    for (auto& single_game_data : players_gamedata) {
        player& current_player = the_players->at(single_game_data[0]);
        game_performance = 0.0;

        current_player.add_record(single_game_data);

        auto teamID = the_db->query("SELECT max(teamID) from gamedata where name = '" +
                                    current_player.name + "';");
#if TEST == 1
        if (teamID.size() == 0) {
            cout << "Did not retrieve the TeamID." << endl;
        }
#endif

        vector<pair<double, double> > mean_sd_pairs{};
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

        double game_variable{0.0};
        double deviation{0.0};
        double num_sds{0.0};

        for (unsigned i = 1; i < mean_sd_pairs.size(); i++) {
            game_variable = atoi(single_game_data[i + 1].c_str());

            if (mean_sd_pairs[i].second != 0) {
                num_sds =
                    (game_variable - mean_sd_pairs[i].first) / mean_sd_pairs[i].second;
            } else {
                num_sds = 0;
            }

            game_performance += num_sds;

            /*
            //This determines the percentile this players' variable scores in. eg: 50th,
            99th, etc
            string R_Query = "pnorm(" + single_game_data[i + 1] + ", mean = " +
                             to_string(mean_sd_pairs[i].first) + ", sd = " +
                             to_string(mean_sd_pairs[i].second) + ")";
            R_Queries.emplace_back(R_Query);
            */
        }

        double percent_minutes = atoi(single_game_data[1].c_str()) / 48.0;
        game_performance *= percent_minutes;

        for (auto& RInside_Query : R_Queries) {
            double variable_performance = RInside->use(RInside_Query);
            game_performance += variable_performance;
        }

        auto update_query = "UPDATE gamedata SET npr = " + to_string(game_performance) +
                            " WHERE gameID = " + gameid + " AND Name = '" +
                            single_game_data[0] + "';";

        result_set.emplace_back(update_query);
    }

    delete the_db;
    return result_set;
}

vector<string> game::generate_PIR() {
    Database* the_db = new Database("../2015.db");
    vector<string> result_set{};

    string query =
        "SELECT points, oreb, dreb, assist, steal, block, ftm, fta, fgm, fga, turnover, "
        "fouls, name FROM gamedata WHERE gameID = " +
        gameid + " and injury = 'NULL';";

    auto players_gamedata = the_db->query(query);
#if TEST == 1
    if (players_gamedata.size() == 0) {
        cout << "Did not retrieve any gamedata for game: " << gameid << endl;
    }
#endif

    for (auto& player : players_gamedata) {
        auto points = atoi(player[0].c_str());
        auto oreb = atoi(player[1].c_str());
        auto dreb = atoi(player[2].c_str());
        auto assist = atoi(player[3].c_str());
        auto steal = atoi(player[4].c_str());
        auto block = atoi(player[5].c_str());
        auto ftm = atoi(player[6].c_str());
        auto fta = atoi(player[7].c_str());
        auto fgm = atoi(player[8].c_str());
        auto fga = atoi(player[9].c_str());
        auto turnover = atoi(player[10].c_str());
        auto fouls = atoi(player[11].c_str());

        int game_performance =
            (points + oreb + dreb + assist + steal + block + (fta / 2)) -
            ((fga - fgm) + (fta - ftm) + turnover + fouls);

        auto update_query = "UPDATE gamedata SET pir = " + to_string(game_performance) +
                            " WHERE gameID = " + gameid + " AND Name = '" + player[12] +
                            "';";

        result_set.emplace_back(update_query);
    }

    delete the_db;
    return result_set;
}