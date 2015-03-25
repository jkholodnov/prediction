#include <stdlib.h>

using namespace std;

int main(int argc, char const *argv[])
{
	/* code */
	string selected_option{""};
	while(selected_option != "X"){
		cout << "What component would you like to run?" << endl << "Options: Scraper, ELO, Simulation, or 'X'" << endl;
		cin >> selected_option;

		if(selected_option == "Scraper"){

		}
		else if(selected_option == "ELO"){

		}
		else if(selected_option == "Simulation"){
			string team1, team2;
			cout << "Which two teams would you like to compare?" << endl;
			cin >> team1 >> team2;
			system("Game_Simulation/./Game_Simulation_Executable team1 team2")
		}
	}
	
	return 0;
}