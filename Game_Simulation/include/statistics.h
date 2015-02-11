#ifndef STATISTICS_H
#define STATISTICS_H

#include <math.h>
#include <vector>
#include <string>
#include <RInside.h>


using namespace std;

class statistics
{
    struct shots{
        double attempted;
        double made;
        double percent_made;
        shots(string a, string m){
            attempted = atoi(a.c_str());
            made = atoi(m.c_str());
            if(attempted == 0){
                percent_made = 0.0;
            }
            else{
                percent_made = made/attempted;
            }
        }
    };

    public:
        statistics(vector<double>& data_vector);
        virtual ~statistics();

        //WORKFLOW: GET MEAN OF THE INT VECTOR -> GET GET SSR VALUES -> GET MEAN OF SSR -> SQRT FOR STDEV//
        double get_mean(const vector<double>& data_vector);
        double get_stdev(double mean_value, const vector<double>& data_vector);

    protected:
    private:
        double mean;
        double stdev;

};

#endif // STATISTICS_H
