#include "../include/games_and_teams.h"

games_and_teams::games_and_teams() { the_db = new Database("../2015.db"); }

games_and_teams::~games_and_teams() { delete the_db; }

/**
 * @brief Initializes teams vector, as well as players map with team abbreviations and
 * player names.
 */
void games_and_teams::initialize_teams() {
    string _query = "SELECT distinct Team1Abbr FROM games;";
    auto team_abbreviations = the_db->query(_query);

    for (auto &team_abbreviation : team_abbreviations) {
        the_teams.emplace_back(team_abbreviation[0]);
    }

    auto player_names = the_db->query("SELECT DISTINCT(name) from gamedata;");
#if TEST == 1
    if (player_names.size() == 0) {
        cout << "Did not retrieve any player names from database. Check db path." << endl;
    }
#endif

    for (auto &player : player_names) {
        the_players.emplace(player[0], player[0]);
    }

#if TEST == 1
    if (the_players.size() == 0) {
        cout << "Did not create any players. Check above function." << endl;
    }
#endif
}

/**
 * @brief Get game metadata, team names, team scores, elos, days. Breaks up all days into
 * parallelizable chunks.
 */
void games_and_teams::load_in_games() {
    string _query = "SELECT DISTINCT day FROM games order by day asc;";
    auto game_days = the_db->query(_query);

#if TEST == 1
    cout << "Pulled " << game_days.size() << " days where games were played." << endl;
#endif

    for (auto &day : game_days) {
        vector<string> games_on_this_day;
        _query =
            "SELECT gameId, Team1Abbr, Team2Abbr, Team1Score, Team2Score, "
            "Team1ELO, Team2ELO FROM games where day = '" +
            day[0] + "';";

        auto games_on_day = the_db->query(_query);
        for (auto &_game : games_on_day) {
            int _score1, _score2;
            team *team1;
            team *team2;

            for (auto &team : the_teams) {
                if (team.getAbbr() == _game[1]) {
                    team1 = &team;
                } else if (team.getAbbr() == _game[2]) {
                    team2 = &team;
                }
            }

            _score1 = atoi(_game[3].c_str());
            _score2 = atoi(_game[4].c_str());

            pair<string, game> gamepair{_game[0],
                                        {_game[0], team1, team2, _score1, _score2}};
            games_map.emplace(gamepair);

            games_on_this_day.emplace_back(_game[0]);
        }
        the_games.emplace_back(games_on_this_day);
    }

    int num_games{0};
    for (auto g : the_games) {
        num_games += g.size();
    }
    cout << "Total of " << num_games << " games loaded into memory." << endl;
}

/**
 * @brief Generates ELO, updates database
 * @details Gets a list of days NBA games were played, splits up games on each
 * day into multiple threads, compares teams, passes back a DB update query,
 * updates sequentially.
 */
int games_and_teams::compute_ELO() {
    int Number_Correct_Ranking{0};

    string days_query =
        "SELECT DISTINCT(day) FROM games WHERE team1elo = 'NULL' or team2elo = 'NULL' "
        "order by day ASC;";

    auto days_needing_elo_updates = the_db->query(days_query);

    for (auto &day : days_needing_elo_updates) {
        cout << day[0] << "\t" << flush;
        vector<future<pair<int, string>>> this_days_games;
        auto games_without_elo = the_db->query(
            "SELECT DISTINCT(gameid) FROM games where day = '" + day[0] + "';");

        for (auto &gameid : games_without_elo) {
            cout << gameid[0] << '\t' << flush;
            game &current_game = games_map.at(gameid[0]);
            this_days_games.emplace_back(
                async(launch::async, &game::generate_Team_ELO, &current_game));
        }

        for (auto &game : this_days_games) {
            auto returned_pair = game.get();
            the_db->query(returned_pair.second);
            Number_Correct_Ranking += returned_pair.first;
            cout << "." << flush;
        }
    }

    /*

    for (auto &game_day : the_games) {
        vector<future<pair<int, string>>> this_days_games;
        for (auto &_game : game_day) {
            game &current_game = games_map.at(_game);

            this_days_games.emplace_back(
                async(launch::async, &game::generate_Team_ELO, &current_game));
        }

        for (auto &game : this_days_games) {
            auto returned_pair = game.get();
            the_db->query(returned_pair.second);
            Number_Correct_Ranking += returned_pair.first;
            cout << "." << flush;
        }
    }
    */
    cout << "\n" << Number_Correct_Ranking << endl;
    return Number_Correct_Ranking;
}

/**
 * @brief Gets all the performance ratings for each player for all games. Updates database
 * @details
 * for each team: Get a list of players
 *      For each player: Get all gamedata they played
 *      Run through it sequentially, generating an update query for each player.
 */
void games_and_teams::compute_NPR() {
    shared_ptr<RInside_Container> R_Inside_Container(new RInside_Container);
    vector<string> update_queries{};

    // Send off async tasks to get database update queries.//

    unordered_map<string, player> *players_map = &the_players;
    for (auto &day : the_games) {
        vector<future<vector<string>>> NPR_updates;
        for (auto &_game : day) {
            game &current_game = games_map.at(_game);
            NPR_updates.emplace_back(async(launch::async, &game::generate_NPR,
                                           &current_game, players_map,
                                           R_Inside_Container));
        }

        for (auto &async_thread : NPR_updates) {
            auto return_result = async_thread.get();

            update_queries.insert(update_queries.end(), return_result.begin(),
                                  return_result.end());

#if TEST == 1
            if (return_result.size() == 0) {
                cout << "generate_NPR did not return anything." << endl;
            }
#endif
        }
    }

#if TEST == 1
    if (update_queries.size() == 0) {
        cout << "Did not get any sql update queries." << endl;
    }
#endif

    for (auto &update : update_queries) {
        cout << "." << flush;
        the_db->query(update);
    }
    cout << endl;
}

/**
 * @brief Updates database to include PIR
 * @details
 *      -Find all games where PIR is null
 *      -Generate update queries for these games
 *      -Update database with the generated queries
 */
void games_and_teams::compute_PIR() {
    shared_ptr<RInside_Container> R_Inside_Container(new RInside_Container);
    vector<string> update_queries{};

    string games_to_update =
        "SELECT DISTINCT(gameid) FROM games WHERE pir = 'NULL' and injury = 'NULL';";

    auto games = the_db->query(games_to_update);

    // Send off async tasks to get database update queries.//
    vector<future<vector<string>>> PIR_updates;
    for (auto &_game : games) {
        game &current_game = games_map.at(_game[0]);
        PIR_updates.emplace_back(
            async(launch::async, &game::generate_PIR, &current_game));
    }

    for (auto &async_thread : PIR_updates) {
        auto return_result = async_thread.get();

        update_queries.insert(update_queries.end(), return_result.begin(),
                              return_result.end());

#if TEST == 1
        if (return_result.size() == 0) {
            cout << "generate_PIR did not return anything." << endl;
        }
#endif
    }

#if TEST == 1
    if (update_queries.size() == 0) {
        cout << "Did not get any sql update queries." << endl;
    }
#endif

    for (auto &update : update_queries) {
        cout << "." << flush;
        the_db->query(update);
    }
    cout << endl;
}