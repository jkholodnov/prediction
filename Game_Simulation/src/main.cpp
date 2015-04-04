#include "../include/dependencies.h"


#include "../include/team.h"
#include "../include/RInside_Container.h"

using namespace std;

int main(int argc, char** argv){
    unique_ptr<Database> predict_db(new Database("../predict.db"));

    auto teamnames = predict_db->query("SELECT DISTINCT team1Abbr from games ORDER BY team1Abbr");
    cout << "Team Abbreviations:" << endl;
    for(auto team:teamnames){
        cout << team[0] << "\t";
    }
    cout << endl;

    vector<team> teams{};
    cout << "Please enter the teams you wish to run simulations on." << endl;

    string team1, team2;
    cin >> team1 >> team2;

    auto is_t1_valid = predict_db->query("SELECT count(*) FROM games WHERE Team1Abbr = '" + team1 + "'");
    auto is_t2_valid = predict_db->query("SELECT count(*) FROM games WHERE Team1Abbr = '" + team2 + "'");

    #if TEST == 1
    if(is_t1_valid[0][0] == "0"){
        cout << "Team 1 abbreviation failed." << endl;
    }
    if(is_t2_valid[0][0] == "0"){
        cout << "Team 2 abbreviation failed." << endl;
    }
    #endif

    if((is_t1_valid[0][0] == "0") || (is_t2_valid[0][0] == "0")){
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

    #if TEST == 1
    if(generate_team_workers.size() != teams.size()){
        cout << "Did not generate enough team workers." << endl;
    }
    #endif

    R_Inside_Container.reset();

    for(i=0; i<generate_team_workers.size();i++){
        generate_team_workers[i].join();
    }


    auto team1_simulations_and_update = teams[0].aggregate_player_scores();
    auto team2_simulations_and_update = teams[1].aggregate_player_scores();
    auto team1_simulated_values = team1_simulations_and_update.first
    auto team2_simulated_values = team2_simulations_and_update.first

    vector<string> update_queries{};
    update_queries.insert(update_queries.end(), team1_simulations_and_update.second.begin(), team1_simulations_and_update.end());
    update_queries.insert(update_queries.end(), team2_simulations_and_update.second.begin(), team2_simulations_and_update.end());

    cout << "There are " << update_queries.size() << " updates that need to be ran." <<3 endl;

    #if TEST == 1
        if(team1_simulated_values.size() != 100){
            cout << "Did not simulate 100 games for team1." << endl;
        }
        if(team2_simulated_values.size() != 100){
            cout << "Did not simulate 100 games for team2." << endl;
        }
        if(team1_simulated_values.size() != team2_simulated_values.size()){
            cout << "Did not simulate the same number of games for each team." << endl;
        }
    #endif

    vector<int> spreads;
    double average_spread{0};
    for(unsigned i=0; i<team1_simulated_values.size(); ++i){
        int spread = team1_simulated_values[i] - team2_simulated_values[i];
        spreads.push_back(spread);
        average_spread+= spread;
    }
    cout << "Average Spread in favor of " << team1 << ": " << average_spread/100 << endl;
    cout << "Minimum Spread: " << *min_element(spreads.begin(), spreads.end()) << "\tMaximum Spread: " << *max_element(spreads.begin(), spreads.end()) << endl;
}
