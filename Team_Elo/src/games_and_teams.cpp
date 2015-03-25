#include "../include/games_and_teams.h"

games_and_teams::games_and_teams()
{
    the_db = new Database("../predict.db");
}

games_and_teams::~games_and_teams()
{
    delete the_db;
}

void games_and_teams::initialize_teams(){
    string _query = "SELECT distinct Team1Abbr FROM games;";
    auto team_abbreviations = the_db->query(_query);


    for(auto& team_abbreviation : team_abbreviations){
        the_teams.emplace_back(team_abbreviation[0]);
    }
}
void games_and_teams::get_games()
{
    string _query = "SELECT gameId, Team1Abbr, Team2Abbr, Team1Score, Team2Score FROM games ORDER BY day ASC;";

    auto games_info = the_db->query(_query);
    int _score1, _score2;

    for(auto& game: games_info)
    {
        team* team1;
        team* team2;

        for(auto& team : the_teams){
            if(team.getAbbr() == game[1]){
                team1 = &team;
            }
            else if(team.getAbbr() == game[2]){
                team2 = &team;
            }
        }
        
        _score1 = atoi(game[3].c_str());
        _score2 = atoi(game[4].c_str());

        the_games.emplace_back(game[0], team1, team2, _score1, _score2);
    }
    cout << "Pulled all games. There are " << the_games.size() << " games to parse." << endl;
}

void games_and_teams::parseGames()
{
    int Number_Correct_Ranking{0};

    for(auto& game:the_games)
    {
        Number_Correct_Ranking += game.update_Team_Ratings(the_db);
    }
    cout << Number_Correct_Ranking << endl;
}
