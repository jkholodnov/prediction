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
        worker_threads.emplace_back(&player::get_player_scores, players[i], R_Inside_Container);
    }

    for(i=0;i<worker_threads.size();i++){
        worker_threads[i].join();
    }

    cout << "Done simulating player scores. Time to combine the team values." << endl;

    delete predict_db;
}

void team::aggregate_player_scores(){
    size_t i,j;
    struct minutes_and_score{
        minutes_and_score(double mins, double scoree){
            minutes = mins;
            score = scoree;
        }
        ~minutes_and_score(){}
        bool operator<(const &minutes_and_score other){
            return minutes < other.minutes;
        }
        double minutes;
        double score;
    };
    vector<double> simulation_scores;
    for(i=0; i<100; i++){
        vector<minutes_and_score> mins_and_scores_vector;
        //calculate the players' points based on lm output.//
        //find the top 5 minutes predicted, sum up their points, return that as the value. 
        for(auto& playerr: players){
            auto the_simulation = playerr.game_simulations[i];
            auto map_of_performances = the_simulation.simulated_performance;

            auto minutes = map_of_performances.find("minutes");
            auto fga = map_of_performances.find("fga");
            auto tpa = map_of_performances.find("tpa");
            auto fta = map_of_performances.find("fta");
            auto oreb = map_of_performances.find("oreb");
            auto assist = map_of_performances.find("assist");
            auto steal = map_of_performances.find("steal");
            auto turnover = map_of_performances.find("turnover");
            auto fouls = map_of_performances.find("fouls");
            auto plus_minus = map_of_performances.find("plus_minus");

            auto minutes_value = minutes->second;
            auto fga_value = fga->second;
            auto tpa_value = tpa->second;
            auto fta_value = fta->second;
            auto oreb_value = oreb->second;
            auto assist_value = assist->second;
            auto steal_value = steal->second;
            auto turnover_value = turnover->second;
            auto fouls_value = fouls->second;
            auto plus_minus_value = plus_minus->second;

            double predicted_score = -0.58433440 + 0.03139078*minutes_value + 0.95074756*fga_value + 0.20738638*tpa_value + 0.78631074*fta_value - 0.07971875*oreb_value - 0.15509996*assist_value - 0.09794145*steal_value + 0.10168076*turnover_value + 0.03700077*fouls_value + 0.08231120*plus_minus_value; 
        
            mins_and_scores_vector.emplace_back(minutes_value, predicted_score);
        }

        sort(mins_and_scores_vector);
        double sum_of_top_5_scores{0.0};
        for(j=0; j<5; j++){
            sum_of_top_5_scores += mins_and_scores_vector[j].score;
        }
    }
}

   