#include "rating_change.h"
#include <math.h>

rating_change::rating_change(double x)
{
    rating = x;
    games_since = {0};
    //ctor
}

rating_change::~rating_change()
{
    //dtor
}

void rating_change::game_passed()
{
    games_since ++;
}

double rating_change::calculate_decayed_rating()
{
    double x = rating * pow(2,(-1*(.1*games_since)));
    return x;
}
