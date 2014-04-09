#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <streambuf>
#include "teams.h"
using namespace std;

class allTeams{

public:
	allTeams(vector<string> teamNames, vector<string> teamUrls){
		int i;
		for(i=0;i<teamNames.size();i++){

		}
	}
	~allTeams(){}
	append(teams x){
		theTeams.push_back(x);
	}
private:
	vector<teams> theTeams;
};