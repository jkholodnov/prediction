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
    
    string _query = "SELECT DISTINCT day FROM games;";
    auto game_days = the_db->query(_query);

    for(auto& day : game_days){
        vector<game> games_on_this_day;
        _query = "SELECT gameId, Team1Abbr, Team2Abbr, Team1Score, Team2Score FROM games where day = '" + day[0] + "';";
        auto games_on_day = the_db->query(_query);
        for(auto& game : games_on_day){
            int _score1, _score2;
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

            games_on_this_day.emplace_back(game[0], team1, team2, _score1, _score2);
        }
        the_games.emplace_back(games_on_this_day);
    }
    cout << "Pulled all games. There are " << the_games.size() << " days of games to parse." << endl;
}

void games_and_teams::parseGames()
{
    size_t i;

    int Number_Correct_Ranking{0};

    for(auto& game_day:the_games)
    {
        vector<future<pair<int,string>>> this_days_games;
        for(i=0; i<game_day.size(); i++){
            this_days_games.emplace_back(async( launch::async, &game::update_Team_Ratings, &game_day[i] ));
        }

        /*
        for(auto& game : game_day){
            schedule(game_parser_threads, boost::bind(&game::update_Team_Ratings, &game))
        }
        */
        //Number_Correct_Ranking += game.update_Team_Ratings(the_db);

        for(auto& game: this_days_games){
            auto returned_pair = game.get();
            the_db->query(returned_pair.second);
            Number_Correct_Ranking += returned_pair.first;
        }
    }
    cout << Number_Correct_Ranking << endl;
}
