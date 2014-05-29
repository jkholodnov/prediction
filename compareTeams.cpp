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
#include "teamavg.h"

int main(int argc, char** argv){//Takes arguments from STDIN to compare. This is opened by a pipe from python.
//====================THIS PART COMPARES 2 INPUTTED TEAMS=================================//
	string toCompare1;
	string toCompare2;
	ifstream in;
	string team1;
	string team2;
	string str1;
	string temp = "";
	string input = "";
	size_t pos;
	string delim = " ";
	float team1value;
	float team2value;
	cin >> toCompare1;
	cin >> toCompare2;
	while(toCompare1 != "exit"){
		//cout << toCompare1 << "~" << toCompare2<<endl;
		team1 = toCompare1;
		team2 = toCompare2;
		pos = team1.find(delim);
		while(pos!=string::npos){
			temp += team1.substr(0,pos)+"_";
			team1.erase(0,pos+delim.length());
			pos = team1.find(delim);
		}
		temp += team1;
		temp += "avg.txt";
		team1 = temp;

		temp = "";
		pos = team2.find(delim);
		while(pos!=string::npos){
			temp += team2.substr(0,pos)+"_";
			team1.erase(0,pos+delim.length());
			pos = team2.find(delim);
		}
		temp += team2;
		temp += "avg.txt";
		team2 = temp;
		temp = "";
		team1="teamavgs/"+team1;
		team2="teamavgs/"+team2;
		transform(team1.begin(), team1.end(), team1.begin(), ::tolower);
		transform(team2.begin(), team2.end(), team2.begin(), ::tolower);
		//========Now team1 and team2 hold the names of the average textfiles.========//
		in.open(team1.c_str());
		if(in.is_open()){ //TAKE THE WHOLE FILE IN AS A STRING
			in.seekg(0,ios::end);  
			str1.reserve(in.tellg());
			in.seekg(0,ios::beg);
		 
			str1.assign((istreambuf_iterator<char>(in)),
			istreambuf_iterator<char>());
		}
		in.close();
		teamavg pp(str1,"xx","xxx");

		in.open(team2.c_str());
		if(in.is_open()){ //TAKE THE WHOLE FILE IN AS A STRING
			in.seekg(0,ios::end);  
			str1.reserve(in.tellg());
			in.seekg(0,ios::beg);
		 
			str1.assign((istreambuf_iterator<char>(in)),
			istreambuf_iterator<char>());
		}
		in.close();
		teamavg gg(str1,"xx","xxx");
		//cout << pp.getftpercent()<<endl;
		//cout << pp.getAllData() <<gg.getAllData();
		float t1ovalue = (gg.getturnover() + pp.getoreb())*pp.getptsperpossession() + (gg.getfoul()-7)*pp.getftpercent();
		float t2ovalue = (pp.getturnover() + gg.getoreb())*gg.getptsperpossession() + (pp.getfoul()-7)*gg.getftpercent();
		float t1dvalue = (pp.getblock() + pp.getsteal() + pp.getdreb()) *gg.getptsperpossession();
		float t2dvalue = (gg.getblock() + gg.getsteal() + gg.getdreb()) *pp.getptsperpossession();
		float t1bc = pp.getsteal() + pp.getblock() + pp.getreb() + gg.getreb();
		float t2bc = gg.getsteal() + gg.getblock() + gg.getreb() + pp.getreb();
		//=================HERE WE NEED TO CALCULATE THE AVERAGE TEAM VALUE BASED ON REGRESSION...
		team1value = pp.getsteal()*.074 + pp.getblock()*.056 + pp.getturnover()*-.015 + pp.getfgpercent()*1.128 + pp.gettppercent()*.758 + t1ovalue*.017 + t1dvalue*-.05 + t1bc*.020 + pp.getptsperpossession()*.024;
		team2value = gg.getsteal()*.074 + gg.getblock()*.056 + gg.getturnover()*-.015 + gg.getfgpercent()*1.128 + gg.gettppercent()*.758 + t2ovalue*.017 + t2dvalue*-.05 + t2bc*.020 + gg.getptsperpossession()*.024;
		team1value*=pp.getbpi();
		team2value*=gg.getbpi();
		//=================COMPARE THE VALUES EACH TEAM HAS, COUT THE HIGHER
		cout << team1value << endl << team2value << endl;
		if(team1value>team2value){
			cout << toCompare1 << endl;
		}
		else{
			cout << toCompare2 << endl;
		}
		cin >> toCompare1;
		cin >> toCompare2;
	}
	exit(0);
}
