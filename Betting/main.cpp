#include "include/dependencies.h"
#include "include/Database.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <Team1> <Team2>" << endl;
    }

    Database* the_db = new Database("../2015.db");

    string team1elo_query = "SELECT currentELO FROM teams WHERE teamID = '" + string(argv[1]) + "';";
    string team2elo_query = "SELECT currentELO FROM teams WHERE teamID = '" + string(argv[2]) + "';";

    auto team1result = the_db->query(team1elo_query);
    auto team2result = the_db->query(team2elo_query);

    double team1elo = stod(team1result[0][0]);
    double team2elo = stod(team2result[0][0]);

    double elo_difference = abs(team1elo - team2elo);

    string all_games = "SELECT count(*) FROM games WHERE abs(team1elo - team2elo) > " + to_string(elo_difference - 10) + " AND abs(team1elo - team2elo) < " + to_string(elo_difference + 10) + ";";
    string correctly_predicted_games = "SELECT count(*) FROM games WHERE abs(team1elo - team2elo) > " + to_string(elo_difference - 10) + 
    									" AND abs(team1elo - team2elo) < " + to_string(elo_difference + 10) + 
    									" AND ((team1elo > team2elo AND team1score > team2score) OR (team1elo < team2elo AND team1score < team2score);";

    cout << all_games << endl;
  	cout << correctly_predicted_games << endl;
    delete the_db;
}