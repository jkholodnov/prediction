#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class teamavg{
public:
	teamavg(){}
	~teamavg(){}
	teamavg(string line,string winstatus,string teamName){
		size_t pos;
		allData = line;
		winorlose = winstatus;
		name = teamName;
		string delim = "-";
		string delim1 = " ";
		string delim2 = "\n";
		pos = line.find(delim);
		fgm = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim.length());
		pos = line.find(delim1);
		fga = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());
	
		pos = line.find(delim);
		tpm = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim.length());
		pos = line.find(delim1);
		tpa = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim);
		ftm = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim.length());
		pos = line.find(delim1);
		fta = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());	
	
		pos = line.find(delim1);
		oreb = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());
		
		pos = line.find(delim1);
		dreb = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());
		pos = line.find(delim1);
		reb = atoi(line.substr(0,pos).c_str());
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
		foul = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());
		pos = line.find(delim1);
		points = atoi(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());
		
		pos = line.find(delim1);
		fgpercent = atof(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		tppercent = atof(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		//ftpercent = atof(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		bpi = atof(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		pos = line.find(delim1);
		possessions = atof(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());
		pos = line.find(delim1);
		ptsperpossession = atof(line.substr(0,pos).c_str());
		line.erase(0,pos+delim1.length());

		possessions = tpa+fga+(fta*.44)+turnover - oreb;
		ptsperpossession = points/possessions;
		xx = ftm;
		yy = fta;
		ftpercent = xx/yy;
	}

	int getfgm(){return fgm;}
	int getfga(){return fga;}
	int gettpm(){return tpm;}
	int gettpa(){return tpa;}
	int getftm(){return ftm;}
	int getfta(){return fta;}
	int getoreb(){return oreb;}
	int getdreb(){return dreb;}
	int getreb(){return reb;}
	int getassist(){return assist;}
	int getsteal(){return steal;}
	int getblock(){return block;}
	int getturnover(){return turnover;}
	int getfoul(){return foul;}
	int getpoints(){return points;}
	float getfgpercent(){return fgpercent;}
	float gettppercent(){return tppercent;}
	float getftpercent(){return ftpercent;}
	void setftpercent(float x){ftpercent = x;}
	float getbpi(){return bpi;}
	int getpossessions(){return possessions;}
	float getptsperpossession(){return ptsperpossession;}
	float getovalue(){}
	float getdvalue(){}
	float getballcontrol(){}
	string getAllData(){return allData;}
	string getWinStatus(){return winorlose;}
	string getName(){return name;}
private:
string allData;
string winorlose;
string name;
int fgm;
int fga;
int tpm;
int tpa;
int ftm;
int fta;
int oreb;
int dreb;
int reb;
int assist;
int steal;
int block;
int turnover;
int foul;
int points;
float fgpercent;
float tppercent;
float ftpercent;
float bpi;
float possessions;
float ptsperpossession;
float xx;
float yy;
};
