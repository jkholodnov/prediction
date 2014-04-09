#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <streambuf>
#include "teamData.h"
using namespace std;

class pageParser{
public:
	~pageParser(){}
	pageParser(string line){
		size_t pos =0;
		
		string delim = "STARTERS";
		string delim1 = "<!-- end sp links -->";
		string delim2 = "class=\"matchup \"";
		string delim3 = "http://espn.go.com/mens-college-basketball/team";
		string delim4 = "\">";
		string delim5 = "</a>";
		string team1Name; 
		string team2Name;
		string temp;
	
		//get the team names//
		pos = line.find(delim2);
		line.erase(0,pos+delim2.length());
		pos = line.find(delim3);
		
		line.erase(0,pos+delim3.length());
		pos = line.find(delim4);
		line.erase(0,pos+delim4.length());
		pos = line.find(delim5);
		team1Name = line.substr(0,pos);
		
		//we have team1 name now.
		pos = line.find(delim3);
		line.erase(0,pos+delim3.length());
		pos = line.find(delim4);
		line.erase(0,pos+delim4.length());
		pos = line.find(delim5);
		team2Name = line.substr(0,pos);
		line.erase(0,pos+delim5.length());
		//cout << "making pp object" << endl;
		//we have team2 name now.
		
		pos = line.find(delim);
		line.erase(0,pos+delim.length());
		
		pos = line.find(delim);
		string team11 = line.substr(0,pos);
		line.erase(0,pos+delim.length());	
		pos = line.find(delim1);
		string team22 = line.substr(0,pos);
		
		line.erase(0,pos+delim1.length());
		team11 = team1Name + "~" + team11;
		team22 = team2Name + "~" + team22;
		
		teamData team1Data(team11);
		teamData team2Data(team22);
		
		team1 = team1Data;
		team2 = team2Data;

		team1Name = team1Data.getName();
		team2Name = team2Data.getName();
		//bonuspoints = The number of additional ball possessions you get multiplied by your avg pts per shot
		t1Offense = (team2Data.getTurnover() + team1Data.getOrebound()) * team1Data.getPtsPerPossession() + (team2Data.getFouls()-7) * team1Data.getPercentFT();
		t2Offense = (team1Data.getTurnover() + team2Data.getOrebound()) * team2Data.getPtsPerPossession() + (team1Data.getFouls()-7) * team2Data.getPercentFT();
		//preventedPoints = blocks + difference between your dreb and their oreb. This gives the number of ball possessions that you took away from the other team. Multiply this by their ptsperposs
		t1Defense = (team1Data.getBlock() + team1Data.getSteal() + team1Data.getDrebound()) * team2Data.getPtsPerPossession();
		t2Defense = (team2Data.getBlock() + team2Data.getSteal() + team2Data.getDrebound()) * team1Data.getPtsPerPossession();
		t1BallControl = (team1Data.getSteal() + team1Data.getBlock() + team1Data.getRebound()-team2Data.getRebound());
		t2BallControl = (team2Data.getSteal() + team2Data.getBlock() + team2Data.getRebound()-team1Data.getRebound());

	}

	void parseData(){

		int j,k;
		string outputFile = "textdatabases/massGameData.txt";
		ofstream out(outputFile.c_str(),ios::app);
		int x = team1.getPts();
		int y = team2.getPts();
		string t1Name = team1.getOutputFile();
		string t2Name = team2.getOutputFile();
		//determine who won
		if(x > y){
			j = 1;
			k = 0;		
		}
		else{j=0;k=1;}
		out << " 0 " << t1Name;
		team1.printGame();
		out << " " << j << " " << t1bpi << " " << t1Offense << " " << t1Defense << " " << t1BallControl << endl << " 1 " <<  t2Name;
		team2.printGame();
		out << " " << k << " " << t2bpi << " " << t2Offense << " " << t2Defense << " " << t2BallControl << endl << endl;
	}
	void printInfo(){
		string textname = team1.getoutputFile();
		string textname1 = team2.getoutputFile();
		textname = "textfiles/"+textname;
		textname1 = "textfiles/"+textname1;
		transform(textname.begin(), textname.end(), textname.begin(), ::tolower);
		transform(textname1.begin(), textname1.end(), textname1.begin(), ::tolower);
		ofstream out1(textname.c_str(),ios::app);
		ofstream out2(textname1.c_str(),ios::app);
		int team1poss = team1.getfga() + team1.gettpa() + team1.getOrebound();
		int team2poss = team2.getfga() + team2.gettpa() + team2.getOrebound();
		float t1ppp = (team1.getPercent3PT()*3) + (team1.getPercentFG()*2);
		float t2ppp = (team2.getPercent3PT()*3) + (team2.getPercentFG()*2);
		
		team1.setBPI(t1bpi);
		team2.setBPI(t2bpi);
		//cout << t1bpi << " " <<t2bpi << " " << team1.getoutputFile() << " " << team2.getoutputFile() << endl;
		team1.printInfo();
		team2.printInfo();
		out1 << " " << t1bpi << " " << team1poss << " " << t1ppp << endl;
		out2 << " " << t2bpi << " " << team2poss << " " << t2ppp << endl;
	}
	teamData getTeam1(){
		return team1;
	}
	teamData getTeam2(){
		return team2;
	}
	string getTeam1Name(){
		return team1Name;
	}
	string getTeam2Name(){
		return team2Name;
	}
	void setT1bpi(double x){
		t1bpi = x;
	}
	void setT2bpi(double x){
		t2bpi = x;
	}
private:
string team1Name;
string team2Name;
teamData team1;
teamData team2;
double t1bpi;
double t2bpi;
float t1Offense;
float t2Offense;
float t1Defense;
float t2Defense;
int t1BallControl;
int t2BallControl;
};
