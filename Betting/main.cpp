#include "include/dependencies.h"
#include "include/Database.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <Team1> <Team2>" << endl;
    }

    Database* the_db = new Database("../2015.db");

    string team1elo_query =
        "SELECT currentELO FROM teams WHERE teamID = '" + string(argv[1]) + "';";
    string team2elo_query =
        "SELECT currentELO FROM teams WHERE teamID = '" + string(argv[2]) + "';";

    auto team1result = the_db->query(team1elo_query);
    auto team2result = the_db->query(team2elo_query);

    double team1elo = stod(team1result[0][0]);
    double team2elo = stod(team2result[0][0]);

    double elo_difference = abs(team1elo - team2elo);

    string all_games = "SELECT count(*) FROM games WHERE abs(team1elo - team2elo) > " +
                       to_string(elo_difference - 10) +
                       " AND abs(team1elo - team2elo) < " +
                       to_string(elo_difference + 10) + ";";

    string correctly_predicted_games =
        "SELECT count(*) FROM games WHERE abs(team1elo - team2elo) > " +
        to_string(elo_difference - 10) + " AND abs(team1elo - team2elo) < " +
        to_string(elo_difference + 10) +
        " AND ((team1elo > team2elo AND team1score > team2score) OR "
        "(team1elo < team2elo AND team1score < team2score));";

    auto all_games_within_elo_range = the_db->query(all_games);
    auto correct_games_within_elo_range = the_db->query(correctly_predicted_games);

    cout << all_games_within_elo_range[0][0];
/*
    double val1 = stod(all_games_within_elo_range[0][0]);
    double val2 = stod(correctly_predicted_games[0][0]);
  */
    double percent_win = 1 / 2;

    if (team1elo > team2elo) {
        cout << argv[1] << " has a " << percent_win << " chance to win.";
    } else {
        cout << argv[2] << " has a " << percent_win << " chance to win.";
    }

    cout << all_games << endl;
    cout << correctly_predicted_games << endl;
    delete the_db;
}