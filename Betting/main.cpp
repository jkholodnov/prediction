#include "include/dependencies.h"
#include "include/Database.h"

using namespace std;

int main(int argc, char** argv) {
    vector<string> databases{"2014.db", "2015.db"};
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

    double x = stod(all_games_within_elo_range[0][0]);
    double y = stod(correct_games_within_elo_range[0][0]);

    double percent_win = y / x;

    delete the_db;

    double team1winpercent = ((team1elo > team2elo) ? percent_win : 1 - percent_win);
    double team2winpercent = 1 - team1winpercent;

    double fractional_odd = (100 / (team1winpercent * 100)) - 1;
    cout << fractional_odd << endl;

    cout << team1winpercent << endl;
    /*


    auto compute_required_win_returns = [](double winpct) -> double {
        double num_wins = 100 * winpct;
        double num_losses = 100 - num_wins;
        double money_lost = num_losses * 100.0;
        double win_requirement = money_lost / num_wins;
        // cout << num_wins << "\t" << num_losses << "\t" << money_lost << "\t"
        //<< win_requirement << endl;
        return win_requirement;
    };

    auto team1_needed = compute_required_win_returns(team1winpercent);
    auto team2_needed = compute_required_win_returns(team2winpercent);

    if (team1_needed > 100) {
        cout << "The money line on " << argv[1] << " must be at least: +" << team1_needed
             << endl;
    } else {
        cout << "The money line on " << argv[1] << " must be at least: -"
             << 10000 / team1_needed << endl;
    }

    if (team2_needed > 100) {
        cout << "The money line on " << argv[2] << " must be at least: +" << team2_needed
             << endl;
    } else {
        cout << "The money line on " << argv[2] << " must be at least: -"
             << 10000 / team2_needed << endl;
    }

    cout << "The difference in team ratings is: " << elo_difference << endl;

    cout << argv[1] << "\t" << team1elo << "\t"
         << ((team1elo > team2elo) ? percent_win : 1 - percent_win) << endl;
    cout << argv[2] << "\t" << team2elo << "\t"
         << ((team2elo > team1elo) ? percent_win : 1 - percent_win) << endl;
    // cout << num_wins << "\t" << num_losses << "\t" << money_lost << "\t" <<
    // win_requirement << endl;
         */
}
