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
    //std::cout << player_names.size() << std::endl;

    for(auto player: player_names){
        players.emplace_back(player[0],team_name);
    }

    vector<future<vector<string>>> game_performance_updates;
    for(i=0;i<players.size();i++){
        game_performance_updates.emplace_back(async( launch::async, &player::get_player_scores, &players[i], R_Inside_Container ));
    }

    for(i=0;i<game_performance_updates.size();i++){
        auto game_performances_to_update = game_performance_updates[i].get();
        for(auto game_performance: game_performances_to_update){
            predict_db->query(game_performance);
        }
    }
    delete predict_db;
}

//This function combines each players simulated scores. The result is a 100-element vector with predicted team scores.//
vector<int> team::aggregate_player_scores()
{
    size_t i;

    cout << "Beginning simulation for " << team_name << endl;
    //TODO : LOOK THRUGH ALL PLAYERS, DETERMINE WHICH ONES WILL MOST LIKELY NOT PLAY NEXT GAME.//

    Database* predict_db = new Database("../predict.db");
    auto last_game_played = predict_db->query("select max(day),gameID from games where team1abbr = '" + team_name + "' or team2abbr = '" + team_name + "';");
    auto player_information = predict_db->query("select name,injury from gamedata where gameid = '" + last_game_played[0][1] + "';");
    
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
    
    vector<int> simulation_scores;
    for(i=0; i<100; i++){
        vector<int> mins_and_scores_vector;

        //ONLY LOOK AT THE PLAYERS WHICH WE THINK WILL PLAY NEXT GAME//
        for(auto& _player : active_players){
            cout << _player.player_name << "#" << active_players.size() << "#" << endl;
            auto simulated_value = _player.simulate_game_scores(i);
            mins_and_scores_vector.emplace_back(simulated_value);
        }

        int team_predicted_points{0};
        for(auto& value : mins_and_scores_vector){
            team_predicted_points += value;
        }
        simulation_scores.push_back(team_predicted_points);
    }
    return simulation_scores;
}