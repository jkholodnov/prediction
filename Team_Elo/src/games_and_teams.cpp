#include "../include/games_and_teams.h"

games_and_teams::games_and_teams()
{

}

games_and_teams::~games_and_teams()
{

}

void games_and_teams::initialize_teams(){
    Database* the_db = new Database("../../predict.db");
    
    string _query = "SELECT distinct Team1Abbr FROM games;";
    auto team_abbreviations = the_db->query(_query);

    for(auto& team_abbreviation : team_abbreviations){
        the_teams.emplace_back(team_abbreviation[0]);
    }

    delete the_db;
}
void games_and_teams::get_games()
{
    Database* the_db = new Database("../../predict.db");
    string _query = "SELECT Id, day, Team1Abbr, Team2Abbr, Team1Score, Team2Score FROM games ORDER BY day ASC;";

    auto games_info = the_db->query(_query);
    int _gameid, _score1, _score2;

    for(auto& game: games_info)
    {
        _gameid = atoi(game[0].c_str());
        //find team by searching thru the_teams

        team* team1;
        team* team2;

        for(auto& team : the_teams){
            if(team.getAbbr == game[2]){
                team1 = &team;
            }
            else if(team.geTAbbr == game[3]){
                team2 = &team;
            }
        }
        
        _score1 = atoi(game[4].c_str());
        _score2 = atoi(game[5].c_str());

        the_games.emplace_back(_gameid, team1, team2, _score1, _score2);
    }

    delete the_db;
}

void games_and_teams::parseGames()
{
    int Number_Correct_Ranking{0};

    for(auto& game:the_games)
    {
        Number_Correct_Ranking += game.update_Team_Ratings();
    }
    cout << Number_Correct_Ranking << endl;
}
