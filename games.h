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
			string base1 = "http://espn.go.com/ncb/recap?gameId=";
			string base2 = "http://espn.go.com/ncb/playbyplay?gameId=";
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
				id = temp;
				string recap = base1+temp;
				string pbp = base2+temp;
				urls.push_back(url);
				urls1.push_back(recap);
				urls2.push_back(pbp);
			}
		}
	}
	~games(){}
	void printUrls(){
		string theoutput1 = "textdatabases/recapurls.txt";
		ofstream out1(theoutput1.c_str(), ios::app);
		for(int j=0;j<urls1.size();j++){
			out1<< urls1[j] << endl;
		}	

		string theoutput = "textdatabases/gameurls.txt";
		ofstream out(theoutput.c_str(),ios::app);
		for(int i=0;i<urls.size();i++){
			out << urls[i] << endl;
		}

		string theoutput2 = "textdatabases/pbpurls.txt";
		ofstream out2(theoutput2.c_str(),ios::app);
		for(int i=0;i<urls2.size();i++){
			out2 << urls2[i] << endl;
		}
		out.close();
		out1.close();
	}	
private:
	string id;
	vector<string> urls;
	vector<string> urls1;
	vector<string> urls2;
};
