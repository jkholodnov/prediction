#include <iostream>
#include "include/games_and_teams.h"

using namespace std;

int main()
{
    games_and_teams all_games{};
    all_games.initialize_teams();
    all_games.get_games();
    all_games.parseGames();
    cout << "Games have been parsed, ELO has been updated in the database." << std::endl;
}
