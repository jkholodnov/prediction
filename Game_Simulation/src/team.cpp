#include "team.h"

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

void team::generate_team_parallel(){
    size_t i;

    Database* predict_db = new Database("../predict.db");
    auto player_names = predict_db->query("SELECT DISTINCT Name FROM gamedata WHERE teamid = '" + team_name + "' ORDER BY Name;");
    //std::cout << player_names.size() << std::endl;

    for(auto player: player_names){
        players.emplace_back(player[0]);
    }
    cout << "Team " << team_name << " has fetched " << players.size() << " players. Proceeding to create player threads." << endl;

    vector<thread> worker_threads;
    for(i=0;i<players.size();i++){
        worker_threads.emplace_back(&player::get_player_scores, players[i]);
    }
    cout << "Player threads created for team " << team_name << ". Waiting for them to join." << endl;
    for(i=0;i<worker_threads.size();i++){
        worker_threads[i].join();
    }
    cout << "Player threads joined for team " << team_name << "." << endl;
    delete predict_db;
}
