#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

#include "include/games_and_teams.h"

using namespace std;
int main()
{
	using namespace std::chrono;
<<<<<<< HEAD
	high_resolution_clock::time_point t1 = high_resolution_clock::now();    

    games_and_teams all_games{};
=======

  	high_resolution_clock::time_point t1 = high_resolution_clock::now();    
  	games_and_teams all_games{};
>>>>>>> Team_Elo_Dev
    all_games.initialize_teams();
    all_games.get_games();
  	high_resolution_clock::time_point t2 = high_resolution_clock::now();    
    all_games.parseGames();
<<<<<<< HEAD
      	high_resolution_clock::time_point t3 = high_resolution_clock::now();    
	duration<double> time_span = duration_cast<duration<double>>(t3 - t1);

	std::cout << "The program took " << time_span.count() << " seconds.";
}
=======
  	high_resolution_clock::time_point t3 = high_resolution_clock::now();    
	
	duration<double> time_span = duration_cast<duration<double>>(t3 - t1);
	std::cout << "The program took " << time_span.count() << " seconds.";
}
>>>>>>> Team_Elo_Dev
