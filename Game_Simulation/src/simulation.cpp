#include "../include/simulation.h"

simulation::simulation(){
	//ctor
}

simulation::~simulation()
{
    //dtor
}

void simulation::simulate_players_performance(const unordered_map<string, statistics>& mean_and_stdevs, const vector<string>& keys_to_map, shared_ptr<RInside_Container> R_Inside_Container)
{
	auto threadID = std::this_thread::get_id();
    auto thread_Hash = std::hash<std::thread::id>()(threadID);
    srand(thread_Hash);
    int performance;

    //TODO: Check if the performance in top 10% or bottom 10%
    /*
    int performance = rand() % 120;
    if(performance < 10){

	}
   	else if(performance > 110){

   	}
   	else{
		performance -= 10;
   	}
   	*/

    for(auto key: keys_to_map){
    	performance = rand() %100;
    	auto statistics_mean_and_stdev =  mean_and_stdevs.find(key);
    	double mean = (statistics_mean_and_stdev->second).mean;
    	double stdev = (statistics_mean_and_stdev->second).stdev;
    	string RInside_Query = "qnorm(." + to_string(performance) + ", mean = " + to_string(mean) + ", stdev = " + to_string(stdev) + ")";
    	//cout << "Mean " << mean << " Stdev " << stdev << "Query " << RInside_Query << endl;
    	auto predicted_value = R_Inside_Container->use(RInside_Query);
    	simulated_performance.emplace(key,predicted_value);
    }

    cout << "Simulated all performances. The size is now: " << simulated_performance.size() << endl;
}