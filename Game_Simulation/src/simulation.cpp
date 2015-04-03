#include "../include/simulation.h"

simulation::simulation(){
	//ctor
}

simulation::~simulation()
{
  //dtor
}

void simulation::simulate_players_performance(const unordered_map<string, statistics>& mean_and_stdevs, const vector<string>& keys_to_map, shared_ptr<RInside_Container> R_Inside_Container, string playname)
{
	auto threadID = std::this_thread::get_id();
    auto thread_Hash = std::hash<std::thread::id>()(threadID);
    srand(thread_Hash);
    int performance = rand() % 100;
    //TODO: Check if the performance in top 10% or bottom 10%

    for(auto key: keys_to_map){
    	auto statistics_mean_and_stdev =  mean_and_stdevs.find(key);
    	double mean = (statistics_mean_and_stdev->second).mean;
    	double stdev = (statistics_mean_and_stdev->second).stdev;
        cout << key << "~" << statistics_mean_and_stdev->first << endl;
    	string RInside_Query = "qnorm(." + to_string(performance) + ", mean = " + to_string(mean) + ", sd = " + to_string(stdev) + ")";
    	//cout << playname << " Number of container uses: " << R_Inside_Container.use_count() <<" Key: " << key << " " << statistics_mean_and_stdev->first << " Mean: " << mean << " Stdev: " << stdev << " Query: " << RInside_Query << endl;
    	auto predicted_value = R_Inside_Container->use(RInside_Query);

        if( (key != "plus_minus") && (predicted_value < 0) ){
            predicted_value = 0;
        }
    	simulated_performance.emplace(key,predicted_value);
    }
}

int simulation::get_simulated_value(string key){
    auto simulated_entity = simulated_performance.find(key);
    int simulated_value{0};
    if(simulated_entity != simulated_performance.end()){
      simulated_value = round(simulated_entity->second);
    }
    return simulated_value;
}