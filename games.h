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

class games{
public:
	games(string line){//Takes in a full webpage
		while(line.length()>0){
			string delim = "/ncb/recap?gameId=";
			string delim1 = "\">";
			string base = "http://espn.go.com/ncb/boxscore?gameId=";
			size_t pos;
			pos = line.find(delim);
			if (pos==string::npos){//if there are no more game urls to find delete the string
				line.erase(0,line.length());
			}
			else{	
				string temp = line.substr(0,pos);
				line.erase(0,pos+delim.length());
		
				pos = line.find(delim1);
				temp = line.substr(0,pos);
				line.erase(0,pos+delim1.length());
				string url = base + temp;
				urls.push_back(url);
			}
		}
	}
	~games(){}
	void printUrls(){
		string theoutput = "textdatabases/gameurls.txt";
		ofstream out(theoutput.c_str(),ios::app);
		for(int i=0;i<urls.size();i++){
			out << urls[i] << endl;
		}
		out.close();
	}

private:
	vector<string> urls;
};
