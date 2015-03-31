#include "../include/dependencies.h"


#include "../include/team.h"
#include "../include/RInside_Container.h"

using namespace std;

int main(int argc, char** argv){
    unique_ptr<Database> predict_db(new Database("predict.db"));

    auto teamnames = predict_db->query("SELECT DISTINCT team1Abbr from games ORDER BY team1Abbr");
    cout << "Team Abbreviations:" << endl;
    for(auto team:teamnames){
        cout << team[0] << "\t";
    }
    cout << endl;

    vector<team> teams{};
    cout << "Please enter the teams you wish to run simulations on." << endl;

    string team1, team2;
    cin >> team1;
    cin >> team2;

    auto is_t1_valid = predict_db->query("SELECT count(*) FROM games WHERE Team1Abbr = '" + team1 + "'");
    auto is_t2_valid = predict_db->query("SELECT count(*) FROM games WHERE Team1Abbr = '" + team2 + "'");

    if((is_t1_valid[0][0] == "0") || (is_t2_valid[0][0] == "0")){
        cout << "You have inputted improper team abbreviations. Please ensure you are using the correct abbreviations." << endl;
        return 0;
    }
    else{
        teams.emplace_back(team1);
        teams.emplace_back(team2);
    }
    vector<std::thread> generate_team_workers;
    size_t i;

    std::shared_ptr<RInside_Container> R_Inside_Container(new RInside_Container);
    for(i=0; i<teams.size();i++){
        generate_team_workers.emplace_back(&team::generate_team_simulations, &teams[i], R_Inside_Container);
    }
    R_Inside_Container.reset();

    for(i=0; i<generate_team_workers.size();i++){
        generate_team_workers[i].join();
    }
    for(auto team: teams){
        cout << (team.players).size() << endl;
    }

    cout << "Completed simulations. Proceeding to aggregate values." << endl;



    auto team1simulated_values = teams[0].aggregate_player_scores();
    auto team2simulated_values = teams[1].aggregate_player_scores();

    vector<int> spreads;
    int average_spread{0};
    for(unsigned i=0; i<team1simulated_values.size(); ++i){
        int spread = team1simulated_values[i] - team1simulated_values[i];
        spreads.emplace_back(spread);
        average_spread+= spread;
    }
    cout << average_spread << endl;
    cout << min(spreads) << "!" << max(spreads) << endl;
}
