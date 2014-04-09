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
#include "bpi.h"
using namespace std;

class allBpis{
public:
	allBpis(string line){
		size_t xxx;
		while(xxx!=string::npos){
			size_t pos;
			string teamName;
			string bpis;		
			string delim="http://espn.go.com/mens-college-basketball/bpi/_/teamId";
			string delim1="\">";
			string delim2="</a>";
			string delim3="<td align=\"right\" class=\"sortcell\">";
			string delim4="</td>";
			double bpid;
			pos=line.find(delim);
			line.erase(0,pos+delim.length());
			pos=line.find(delim1);
			line.erase(0,pos+delim1.length());
			pos=line.find(delim2);
			teamName=line.substr(0,pos);
			line.erase(0,pos+delim2.length());
			//got the teamname, lets find the matching bpi;
			pos = line.find(delim3);
			line.erase(0,pos+delim3.length());
			pos=line.find(delim4);
			bpis = line.substr(0,pos);
			bpid = atof(bpis.c_str());
			line.erase(0,pos+delim4.length());
			bpi current(teamName,bpid);
			bpiv.push_back(current);

			string eofCheck= "Grambling St";
			xxx = line.find(eofCheck);
		}
	}
	~allBpis(){}
	void printBpis(){
		for(int i=0;i<bpiv.size();i++){
			bpiv[i].printInfo();
		}
	}
	int getSize(){return bpiv.size();}
	vector<bpi> getVector(){
		return bpiv;
	}
private:
	vector<bpi> bpiv;
};
