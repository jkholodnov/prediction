#include "../include/team.h"

team::team(string abbr) { team_Abbreviation = abbr; }

team::~team() {
    // dtor
}

void team::init(string abbr) { team_Abbreviation = abbr; }

double team::get_Rating() { return rating; }

void team::update_Rating(double update_by) { rating += update_by; }

string team::getAbbr() { return team_Abbreviation; }
