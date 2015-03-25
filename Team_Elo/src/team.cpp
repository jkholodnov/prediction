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

void team::update_Rating(double update_by)
{
    //decay the old rating//
    bonus_Rating *= pow(2,-.066);
    //Add the new rating//
    bonus_Rating += update_by;

}

string team::getAbbr()
{
    return team_Abbreviation;
}
