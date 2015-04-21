#include "include/dependencies.h"

#include "include/games_and_teams.h"

using namespace std;

int main() {
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    games_and_teams all_games{};

    all_games.initialize_teams();

#if TEST == 1
    cout << "Initialize Teams : Completed" << endl;
    if (all_games.the_teams.size() == 0) {
        cout << "Did not initialize any teams." << endl;
    }
#endif
    all_games.load_in_games();

#if TEST == 1
    cout << "Load in games : Completed" << endl;
    if (all_games.the_games.size() == 0) {
        cout << "Did not initialize any games." << endl;
    }
#endif
    int number_of_correct_predictions{0};
    number_of_correct_predictions = all_games.generate_ELO();

#if TEST == 1
    cout << "Generate ELO : Completed" << endl;
    if (number_of_correct_predictions == 0) {
        cout << "Did not predict any games correctly... Something went wrong with "
                "generate_ELO function." << endl;
    }
#endif

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout << "The program took " << time_span.count() << " seconds to generate ELO."
         << endl;

    all_games.generate_Performance_Rating();

    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t3 - t2);
    cout << "The program took " << time_span.count()
         << " seconds to generate Performance Rating." << endl;
}
