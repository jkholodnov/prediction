#include "include/dependencies.h"

#include "include/games_and_teams.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Correct usage: " << argv[0] << " <Elo 1/0> <NPR 1/0> <PIR 1/0>" << endl;
        return 1;
    }

    bool elo = (string(argv[1]) == "1");
    bool npr = (string(argv[2]) == "1");
    bool pir = (string(argv[3]) == "1");

    using namespace std::chrono;
    high_resolution_clock::time_point t1, t2, t3;
    duration<double> time_span;

    games_and_teams all_games{};
    cout << "asdf" << endl;
    all_games.initialize_teams();

#if TEST == 1
    cout << "Initialize Teams : Completed" << endl;
    if (all_games.the_teams.size() == 0) {
        cout << "Did not initialize any teams." << endl;
    }
#endif

    all_games.load_in_games();

#if TEST == 1
    if (all_games.the_games.size() == 0) {
        cout << "Did not initialize any games." << endl;
    }
#endif
    int number_of_correct_predictions{0};

    if (elo) {
        cout << "Computing ELO ratings for teams." << endl;
        t1 = high_resolution_clock::now();
        number_of_correct_predictions = all_games.compute_ELO();

        t2 = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Took " << time_span.count() << " seconds to compute ELO." << endl;
    }

    if (pir) {
        cout << "Computing PIR ratings for players." << endl;
        t1 = high_resolution_clock::now();
        all_games.compute_PIR();

        t2 = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Took " << time_span.count() << " seconds to compute PIR" << endl;
    }

    if (npr) {
        cout << "Computing NPR ratings for players." << endl;
        t1 = high_resolution_clock::now();
        all_games.compute_NPR();

        t2 = high_resolution_clock::now();
        time_span = duration_cast<duration<double>>(t2 - t1);
        cout << "Took " << time_span.count() << " seconds to generate NPR" << endl;
    }
}
