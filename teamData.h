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
using namespace std;

class teamData{
public:
	~teamData(){}
	teamData(){
	}
	teamData(string line){
		size_t pos =0;
		string delim = "colspan=\"2\"></td><td>";
		string delim1 = "</td><td>";
		string delim2 = "-";
		string delim3 = " ";
		string delim4 = "~";
		string temp;
		string temp1;
		pos = line.find(delim4);
		name = line.substr(0,pos);
		temp = name;
		pos = temp.find(delim3);
		outputFile = "";
		while(pos!=string::npos){
			outputFile += temp.substr(0,pos)+"_";
			temp.erase(0,pos+delim3.length());
			pos = temp.find(delim3);
		}
		outputFile += temp;
		outputFile += "team.txt";
		pos = line.find(delim);
		line.erase(0,pos+delim.length()); //delete everything before the data section;
			
		pos = line.find(delim1);
		fgma = line.substr(0,pos);
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		tpma = line.substr(0,pos);
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		ftma = line.substr(0,pos);
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		orebound = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		drebound = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		rebound = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		assist = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		steal = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		block = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		turnover = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		fouls = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		pts = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = fgma.find(delim2);
		int tempfgm = atoi(fgma.substr(0,pos).c_str());
		fgma.erase(0,pos+delim2.length());
		int tempfga = atoi(fgma.c_str());
	
		pos = tpma.find(delim2);
		tpm = atoi(tpma.substr(0,pos).c_str());
		tpma.erase(0,pos+delim2.length());
		tpa = atoi(tpma.c_str());	

		pos = ftma.find(delim2);
		ftm = atoi(ftma.substr(0,pos).c_str());
		ftma.erase(0,pos+delim2.length());
		fta = atoi(ftma.c_str());
		//separate 3pt and 2pt shots
		fgm = tempfgm - tpm;
		fga = tempfga - tpa;
		
		percentFT = ftm/fta;
		percent3PT = tpm/tpa;
		percentFG = fgm/fga;
		ptsPerPossession = (percent3PT * 3)+(percentFG*2);
		possessions = fga + tpa - orebound;
	}
	//========================GETTERS / SETTERS========================//
	void setBPI(double x){bpi=x;}
	double getBPI(){return bpi;}
	string getName(){return name;}
	void setName(string x){name = x;}
	string getFgma(){return fgma;}
	string getTpma(){return tpma;}
	string getFtma(){return ftma;}
	int getOrebound(){return orebound;}
	int getDrebound(){return drebound;}
	int getRebound(){return rebound;}
	int getAssist(){return assist;}
	int getSteal(){return steal;}
	int getBlock(){return block;}
	int getTurnover(){return turnover;}
	int getFouls(){return fouls;}
	int getPts(){return pts;}
	string getOutputFile(){return outputFile;}
	float getPtsPerPossession(){
		return ptsPerPossession;
	}
	float getfga(){return fga;}
	float gettpa(){return tpa;}
	int getposessions(){return possessions;}
	float getPercentFT(){return percentFT;}
	float getPercent3PT(){return percent3PT;}
	float getPercentFG(){return percentFG;}
	string getoutputFile(){return outputFile;}
	void printInfo(){ //This function prints out all of the total game data to files named after the schools name. (Florida State Seminoles = florida_stateteam.txt)
		size_t pos =0;
		string textname = "textfiles/"+outputFile;
		transform(textname.begin(), textname.end(), textname.begin(), ::tolower);
		ofstream out(textname.c_str(),ios::app);
		out << fgm << "-" << fga << " " << tpm << "-" << tpa << " " << ftm << "-" << fta << " " << orebound << " " << drebound << " " << rebound<< " " << assist << " " << steal << " " << block << " " <<turnover << " " << fouls << " " << pts << " " << percentFG << " " <<percent3PT << " " << percentFT;
	}
	void printGame(){
		string outputtingFile = "massGameData.txt";
		ofstream out(outputtingFile.c_str(),ios::app);
		out << fgm << "-" << fga << " " << tpm << "-" << tpa << " " << ftm << "-" << fta << " " << orebound << " " << drebound << " " << rebound<< " " << assist << " " << steal << " " << block << " " <<turnover << " " << fouls << " " << pts << " " << percentFG << " " <<percent3PT << " " << percentFT;
		
	}

	//========================OPERATOR OVERLOADS========================//
	bool operator == (teamData x) const{
		if(name == x.getName()){
			return true;
		}
		else{
			return false;		
		}	
	}
	const teamData & operator = (const teamData & rhs){
	       	if (this == &rhs){
		    return *this;
	    	}
		else{
			percentFT=rhs.percentFT;
			percent3PT=rhs.percent3PT;
			percentFG=rhs.percentFG;
			fgm=rhs.fgm;
			fga=rhs.fga;
			tpm=rhs.tpm;
			tpa=rhs.tpa;
			ftm=rhs.ftm;
			fta=rhs.fta;
			fgma=rhs.fgma;
			tpma=rhs.tpma;
			ftma=rhs.ftma;
			orebound=rhs.orebound;
			drebound=rhs.drebound;
			rebound=rhs.rebound;
			assist=rhs.assist;
			steal=rhs.steal;
			block=rhs.block;
			turnover=rhs.turnover;
			fouls=rhs.fouls;
			pts=rhs.pts;
			name=rhs.name;
			outputFile = rhs.outputFile;
		}
		return *this;
	}

private:
float percentFT;
float percent3PT;
float percentFG;
float fgm; //free throw made
float fga; //free throw attempt
float tpm; //3pt made
float tpa; //3pt attempt
float ftm; //free throw made
float fta; //free throw attempt
string fgma;
string tpma;
string ftma;
int orebound;
int drebound;
int rebound;
int assist;
int steal;
int block;
int turnover;
int fouls;
int pts;
string name;
string outputFile;
double bpi;
float ptsPerPossession;
int possessions;
};
