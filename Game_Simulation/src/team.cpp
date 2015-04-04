#include "../include/team.h"

team::team(string teamname)
{
    team_name = teamname;

}

team::~team()
{
    //dtor
}

team::team(const team& other)
{
    //copy ctor
    team_name = other.team_name;
    players = other.players;
}

team::team(team&& other) :
    team_name(std::move(other.team_name)),
    players(std::move(other.players))
{
    //MOVE CONSTRUCTOR//
}

void team::generate_team_simulations(shared_ptr<RInside_Container> R_Inside_Container)
{
    size_t i;

    Database* predict_db = new Database("../predict.db");
    auto player_names = predict_db->query("SELECT DISTINCT Name FROM gamedata WHERE teamid = '" + team_name + "' ORDER BY Name;");
    
    #if TEST == 1
    if(player_names.size() == 0){
        cout << "No player information was retrieved." << endl;
    }
    #endif

    for(auto player: player_names){
        players.emplace_back(player[0],team_name);
    }

    #if TEST == 1
        if(players.size() != player_names.size()){
            cout << "Did not create enough player objects for team: " << team_name << endl;
        }
    #endif

    vector<future<vector<string>>> game_performance_updates;
    for(i=0;i<players.size();i++){
        game_performance_updates.emplace_back(async( launch::async, &player::get_player_scores, &players[i], R_Inside_Container ));
    }

    #if TEST == 1
        if(game_performance_updates.size() != players.size()){
            cout << "Did not launch enough get_player_scores threads asynchronously." << endl;
        }
    #endif


    vector<string> performance_updates{};

    for(i=0;i<game_performance_updates.size();i++){
        auto game_performances_to_update = game_performance_updates[i].get();
        performance_updates.insert(performance_updates.end(),game_performances_to_update.begin(), game_performances_to_update.end());
    }

    #if TEST == 1
        if(performance_updates.size() == 0){
            cout << "Did not generate any performance updates." << endl;
        }
    #endif

    for(auto& update_query: performance_updates){
        predict_db -> query(update_query);
    }
    delete predict_db;
}

//This function combines each players simulated scores. The result is a 100-element vector with predicted team scores.//
pair<vector<int>,vector<string>> team::aggregate_player_scores()
{
    size_t i;
    //TODO : LOOK THRUGH ALL PLAYERS, DETERMINE WHICH ONES WILL MOST LIKELY NOT PLAY NEXT GAME.//

    Database* predict_db = new Database("../predict.db");
    auto last_game_played = predict_db->query("select max(day),gameID from games where team1abbr = '" + team_name + "' or team2abbr = '" + team_name + "';");
    auto player_information = predict_db->query("select name,injury from gamedata where gameid = '" + last_game_played[0][1] + "';");
    
    #if TEST == 1
        if(player_information.size() == 0){
            cout << "Did not select any players from the last game played. Check database status." << endl;
        }
    #endif

    vector<player> active_players;
    vector<player> scratched_players;
    vector<pair<player,string>> injured_players;

    for(auto& player:player_information){
        if(player[1] == "NULL"){
            //the player did play last game
            for(auto& _player: players){
                if(_player.player_name == player[0]){
                    active_players.emplace_back(_player);
                }
            }
        }
        else if(player[1] == "DNP COACH'S DECISION"){
            //the player was scratched. 
            //figure out the chance that the scratched player will return the next game.
            for(auto& _player: players){
                if(_player.player_name == player[0]){
                    scratched_players.emplace_back(_player);
                }
            }
        }
        else{
            //the player was injured. Find out how many games he has been out. Do a check to find avg number of games missed for that injury.
            for(auto& _player: players){
                if(_player.player_name == player[0]){
                    injured_players.emplace_back(_player,player[1]);
                }
            }
        }
    }
    
    #if TEST == 1
        if(active_players.size() == 0){
            cout << "There are no active players... Something is wrong.";
        }
        if(scratched_players.size() == 0){
            cout << "There were no scratched players in " << team_name << "'s previous game." << endl;
        }
        if(injured_players.size() == 0){
            cout << "There were no injured players in " << team_name << "'s previous game." << endl;
        }
    #endif


    vector<int> simulation_scores;
    for(i=0; i<100; i++){
        int team_predicted_points{0};

        //ONLY LOOK AT THE PLAYERS WHICH WE THINK WILL PLAY NEXT GAME//
        for(auto& _player : active_players){
            auto simulated_value = _player.simulate_game_scores(i);
            team_predicted_points += simulated_value;
        }

        #if TEST == 1
            if(team_predicted_points == 0){
                cout << "Did not simulate any points scored. Something is wrong with simulate_game_scores." << endl;
            }
        #endif

        simulation_scores.push_back(team_predicted_points);
    }
    return simulation_scores;
}