#include "../include/team.h"

team::team(string teamname)
{
    team_name = teamname;

}

team::~team()
{
    //dtor
}

team::team(const team& other){
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

void team::generate_team_simulations(shared_ptr<RInside_Container> R_Inside_Container){
    size_t i;

    Database* predict_db = new Database("predict.db");
    auto player_names = predict_db->query("SELECT DISTINCT Name FROM gamedata WHERE teamid = '" + team_name + "' ORDER BY Name;");
    //std::cout << player_names.size() << std::endl;

    for(auto player: player_names){
        players.emplace_back(player[0]);
    }
    cout << "Team " << team_name << " has fetched " << players.size() << " players. Proceeding to create player threads." << endl;

    vector<thread> worker_threads;
    for(i=0;i<players.size();i++){
        worker_threads.emplace_back(&player::get_player_scores, &players[i], R_Inside_Container);
    }

    for(i=0;i<worker_threads.size();i++){
        worker_threads[i].join();
    }

    cout << "Done simulating player scores. Time to combine the team values."  << players.size() << endl;

    delete predict_db;
}

vector<double> team::aggregate_player_scores(){
    size_t i;

    cout << "Starting loop" << endl;
    vector<double> simulation_scores;
    //TODO : LOOK THROUGH ALL PLAYERS, DETERMINE WHICH ONES WILL MOST LIKELY NOT PLAY NEXT GAME.//

    Database* predict_db = new Database("predict.db");
    auto last_game_played = predict_db->query("select max(day),id from games where team1abbr = '" + team_name + "' or team2abbr = '" + team_name + "';");
    auto player_information = predict_db->query("select name,injury from gamedata where gameid = '" + last_game_played[0][1] + "';");
    
    vector<player> active_players;

    for(auto& player:player_information){
        if(player[1] == "NULL"){
            //the player did play last game
            for(auto& _player: players){
                if(_player.player_name == player[0]){
                    active_players.emplace_back(_player);
                }
            }
        }
        else{
            if(player[1].find("DNP COACH'S DECISION") == string::npos){
                //the player was injured. Find out how many games he has been out. Do a check to find avg number of games missed for that injury.
                
            }
            else{
                //the player was scratched. 
                //figure out the chance that the scratched player will return the next game.
            }
        }
    }

    for(i=0; i<1; i++){
        vector<pair<int, int>> mins_and_scores_vector;

        //ONLY LOOK AT THE PLAYERS WHICH WE THINK WILL PLAY NEXT GAME//
        for(auto& _player : active_players){
            auto simulated_value = _player.simulate_game_scores(i);
            mins_and_scores_vector.emplace_back(simulated_value);
        }

        int team_predicted_points{0};
        int team_predicted_turnovers{0};
        for(auto& pair : mins_and_scores_vector){
            team_predicted_points += pair.second;
            cout << pair.second<< "~";
        }
        cout << endl;

        simulation_scores.push_back(team_predicted_points);
        cout << "Simulated score of : " << team_predicted_points << " for team " << team_name << endl;
    }
    return simulation_scores;
}