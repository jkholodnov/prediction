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
    auto last_game_played = predict_db->query("select max(day),id from games where team1abbr = " + team_name + " or team2abbr = " + team_name + ";");

    cout << last_game_played[0][1] << endl;


    for(i=0; i<1; i++){
        vector<pair<int, int>> mins_and_scores_vector;
        //calculate the players' points based on lm output.//
        //find the top 5 minutes predicted, sum up their points, return that as the value. 
        auto cmp_players = [&i](player const & a, player const & b) 
        {
            auto the_simulation = a.game_simulations[i];
            auto map_of_performances = the_simulation.simulated_performance;
            auto minutes = map_of_performances.find("minutes");
            auto a_minutes_value = minutes->second;

            auto bthe_simulation = b.game_simulations[i];
            auto bmap_of_performances = bthe_simulation.simulated_performance;
            auto bminutes = bmap_of_performances.find("minutes");
            auto b_minutes_value = bminutes->second;
            return a_minutes_value > b_minutes_value;
        };

        sort(players.begin(), players.end(), cmp_players);



        //ONLY LOOK AT THE PLAYERS WHICH WE THINK WILL PLAY NEXT GAME//
        for(auto& _player : players){
            auto simulated_value = _player.simulate_game_scores(i);
            mins_and_scores_vector.emplace_back(simulated_value);
        }

        double SUM_OF_TOP_12_MINUTES_SCORES{0.0};

        for(auto& pair : mins_and_scores_vector){
            SUM_OF_TOP_12_MINUTES_SCORES += pair.second;
            cout << pair.second<< "~";
        }
        cout << endl;

        simulation_scores.push_back(SUM_OF_TOP_12_MINUTES_SCORES);
        cout << "Simulated score of : " << SUM_OF_TOP_12_MINUTES_SCORES << " for team " << team_name << endl;
    }
    return simulation_scores;
}