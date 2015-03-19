#include "../include/statistics.h"

statistics::statistics(vector<double>& data_vector)
{
    mean = get_mean(data_vector);
    stdev = get_stdev(mean,data_vector);
}

statistics::~statistics()
{
    //dtor
}

double statistics::get_mean(const vector<double>& data_vector){
    double mean{0};
    for(auto& i: data_vector){
        if(i!=-1){
            mean += i;
        }
        else{
        	//cout << "Player did not play this game" << endl;
        }
    }
    mean /= data_vector.size();
    return mean;
}

double statistics::get_stdev(double mean_value,const vector<double>& data_vector){

    double value_minus_mean{0.0};
    vector<double> residuals{};
    for(auto& i: data_vector){
        value_minus_mean = i-mean;
        residuals.emplace_back(pow(value_minus_mean,2));
    }
    double stdev_squared = get_mean(residuals);
    return sqrt(stdev_squared);
}
