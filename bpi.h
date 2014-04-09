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
using namespace std;

class bpi{
public:
	bpi(string name,double x){
		teamName=name;
		bpis=x;
	}
	~bpi(){}
	string getTeam(){
		return teamName;	
	}
	double getBpi(){
		return bpis;
	}
	void printInfo(){
		cout << teamName << "~" << bpis << endl;	
	}
	const bpi & operator = (const bpi & rhs){
	       	if (this == &rhs){
		    return *this;
	    	}
		else{
			teamName = rhs.teamName;
			bpis = rhs.bpis;
		}
		return *this;
	}
private:
	string teamName;
	double bpis;
};
