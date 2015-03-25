#include "../include/team.h"

team::team(string abbr)
{
    team_Abbreviation = abbr;
}

team::~team()
{
    //dtor
}

void team::init(string abbr)
{
    team_Abbreviation = abbr;
}

double team::get_Rating()
{
    return base_Rating + bonus_Rating;
}

void team::update_Rating(double update_by,double t1Rating,double t2Rating, string t1, string t2)
{

    //Decay the old rating//

    bonus_Rating *= pow(2,-.066);
    //Add the new rating//
    bonus_Rating += update_by;

    string _query = "UPDATE games SET team1ELO = " + to_string(t1Rating) + ", team2ELO = " + to_string(t2Rating) + " WHERE Team1Abbr = '" + t1 + "' AND Team2Abbr = '" + t2 + "';";
    
    Database* the_db = new Database("../../predict.db");
    the_db->query(_query);
    delete the_db;
}

string team::getAbbr()
{
    return team_Abbreviation;
}
