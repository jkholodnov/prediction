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
//FEED THIS FUNCTION THE RAW WEBPAGE WITH ALL OF THE TEAM URL'S//
class teams{
public:
	~teams(){}
	teams(string line){
		size_t pos =0;
		
		while(line.length()>0){			
			string temp;
			string temp1;
			string delim = "http://espn.go.com/mens-college-basketball/team/";
			string delim1 = "\">";
			string delim2 = "</a>";
			pos = line.find(delim);
			if(pos==string::npos){
				line.erase(0,line.length());				
			}
			else{
				line.erase(0,pos+delim.length());
	
				pos = line.find(delim1);
				temp1 = line.substr(0,pos);
				line.erase(0,pos+delim1.length());
				temp = delim+temp1;		
				url.push_back(temp);
			}

			pos = line.find(delim2);
			temp = line.substr(0,pos);
			line.erase(0,pos+delim2.length());
			teamNames.push_back(temp);
		}
	}
	vector<string> geturls(){
		return url;
	}
	vector<string> getTeamNames(){
		return teamNames;
	}
private:
vector<string> url;
vector<string> teamNames;
};