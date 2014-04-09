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
#include "pageParser.h"
#include "teams.h"
#include "games.h"
#include "allBpis.h"
#include "team.h"
#include "teamavg.h"

using namespace std;

int main(int argc, char** argv) {
	size_t pos;
	if(argc==0 || argc ==1){
		cout << "No command line arguments were specified." << endl;
		return 0;
	}
	
	//COMMAND LINE ARGUMENTS//
	string getTeamUrls = argv[1]; //gets the urls to team pages
	string getGameUrls = argv[2]; //gets the urls to each team's games
	string getBpi = argv[3]; //gets the bpi of each team. stores in vector
	string getGameData = argv[4]; //gets game html data, parses into massgamedata.txt
	string getTeamGames = argv[5]; //get all of team games, puts into a text file
	string getTeamAverages = argv[6]; //averages all of team games, creates an average txt file
	
	//DELETE OLD ENTRIES SO THAT OUR DATA IS UP TO DATE//
	if(getTeamUrls=="1"){
		remove("textdatabases/teamurls.txt");
		remove("textdatabases/teams.txt");	
	}
	if(getGameUrls=="1"){
		remove("textdatabases/games.txt");
		remove("textdatabases/gameurls.txt");
		remove("textdatabases/gameUrlsParsed.txt");
	}
	if(getBpi=="1"){
		remove("textdatabases/bpi.txt");
	}
	if(getGameData=="1"){
		remove("textdatabases/massgamedata.txt");
		remove("textdatabases/websitedata.txt");
	}
	if(getTeamGames == "1"){
		system("exec rm -r textfiles/*");
	}
	if(getTeamAverages=="1"){
		system("exec rm -r teamavgs/*");
	}
	//VECTOR DECLARATIONS//
	vector<games> all_Games;
	vector<string> teamUrlss;
	vector<pageParser> each_Game;
	vector<string> gameUrls;
	vector<string> theTeams;
	vector<double> theBpis;
	vector<bpi> bpisVector;
	vector<string> teamNames;
	vector<team> teamSeasonAvgs;
	vector<teamavg> teamavgs;

	//STRING DECLARATIONS//
	string team1;
	string team2;
	string temp;
	string str;
	string str1;
	string action;
	string delim;
	string delim1;
	string delim2;
	string delim3;
	string delim4;
	string t1Name;
	string t2Name;
	string tempteamname;
	string tempdelim;
	string nameofteamfile;
	string tempOutputFileName;
	string teamPlaying;
	string averageData;
	string winner;
	string teamstxt = "textdatabases/teams.txt";
	string gamestxt = "textdatabases/games.txt";
	string massgamefile = "textdatabases/massGameData.txt";
	string gamesparsed = "textdatabases/gameUrlsParsed.txt";
	string bpitxt = "textdatabases/bpi.txt";
	string gamedata = "textdatabases/websitedata.txt";

	//INT DECLARATIONS//
	int i,j,k;
	int t1pos=0;
	int t2pos=0;
	
	//OBJECT DECLARATIONS//
	teamData temp1;
	teamData temp2;
	//OFSTREAM DECLARATIONS//
	ofstream out2("textdatabases/gameUrlsParsed.txt",ios::app);
	ofstream out3("textdatabases/massGameData.txt",ios::app);
	ofstream out4("textdatabases/massDataAverages.txt",ios::app);
	ofstream out5("debug.txt");
	ofstream out6("textdatabases/massDataAverages.txt",ios::app);
	
	//IFSTREAM DECLARATIONS//
	
	ifstream in;
	//THIS NEXT IF GETS ALL THE TEAM URLS AND STORES THEM IN TEAMURLS.TXT ONLY IF CMD LINE ARG1="1"
	if(getTeamUrls=="1"){
		action = "python getData.py";
		system(action.c_str());
		cout << "FINISHED GETTEAMS CALL" <<endl;
		//CALL PYTHON Module: getData
		

		in.open(teamstxt.c_str());
		//GET ALL OF THE TEAM URL'S INTO VECTOR//
		if(in.is_open()){ 
			in.seekg(0,ios::end);  
			str.reserve(in.tellg());
			in.seekg(0,ios::beg);
			 
			str.assign((istreambuf_iterator<char>(in)),
					istreambuf_iterator<char>());
		}
		in.close();
		
		teams theTeams(str);
		teamUrlss = theTeams.geturls();	
		teamNames = theTeams.getTeamNames();

		//ALL TEAM URLS AND NAMES ARE IN THEIR RESPECTIVE VECTORS//
		ofstream out1("textdatabases/teamurls.txt",ios::app);
		//GO THROUGH EACH URL AND PRINT TO TEAMURL.TXT//
		for(i=0;i<teamUrlss.size();i++){
			temp = teamUrlss[i];
			out1 << temp << endl;
		}
		out1.close();
	}
/*
###############################################################################################################
###																										   ####
###			   		WE NOW HAVE TEAM NAMES AND THEIR URLS STORED IN RESPECTIVE VECTORS 					   ####
###																										   ####
###############################################################################################################
*/	


	if(getGameUrls=="1"){ //THIS NEXT IF GETS ALL GAME URLS AND STORES IN GAMEURLS.TXT, IT THEN PARSES AND REMOVES DUPLICATES, STORING IN GAMEURLSPARSED.TXT
		action = "python getGameUrls.py";
		system(action.c_str());
		
		in.open(gamestxt.c_str());
		if(in.is_open()){ 
			in.seekg(0,ios::end);  
			str.reserve(in.tellg());
			in.seekg(0,ios::beg);
			 
			str.assign((istreambuf_iterator<char>(in)),
					istreambuf_iterator<char>());
		}
		in.close();
		
		while(str.length()>0){
			delim1 = "</html>";
			pos=str.find(delim1);
			temp=str.substr(0,pos);
			str.erase(0,pos+delim1.length());
			games current(temp);
			all_Games.push_back(current);
		}
		cout << "All of the game urls have been parsed" << endl;
		//PUT ALL OF THE GAME URLS INTO ONE FILE: gameUrlFile.txt
		for(i=0;i<all_Games.size();i++){
			all_Games[i].printUrls();
		}
		//GAMEURLS.TXT IS NOW POPULATED//

		action="textdatabases/gameurls.txt";
		in.open(action.c_str());
		if(in.is_open()){ 
			in.seekg(0,ios::end);  
			str.reserve(in.tellg());
			in.seekg(0,ios::beg);
			 
			str.assign((istreambuf_iterator<char>(in)),
					istreambuf_iterator<char>());
		}
		in.close();

		delim = "\n";
		i=0;
		while(str.length()>0){
			bool invector=false;
			pos = str.find(delim);
			temp = str.substr(0,pos);
			str.erase(0,pos+delim.length());
			for(i=0;i<gameUrls.size();i++){
				if(gameUrls[i]==temp){
					invector = true;
				}
			}
			if (invector==false){
				gameUrls.push_back(temp);
			}
			//cout << gameUrls.size()<<endl;
		}
		for(i=0;i<gameUrls.size();i++){
			out2 << gameUrls[i] << endl;
		}
		out2.close();
	}
	//ALL OF THE GAME URLS ARE NOW IN GAMEURLSPARSED.TXT//
	

	//==================GET ALL THE BPIS FROM WEBSITE, PARSE INTO VECTORS===================//
/*
###############################################################################################################
###																										   ####
###					WE NOW HAVE THE URLS FOR EVERY GAME IN GAMEURLSPARSED.TXT							   ####
###																										   ####
###############################################################################################################
*/
	if(getBpi=="1"){
		action = "python getBPI.py";
		system(action.c_str());
		//parse all of the team bpis//
		in.open(bpitxt.c_str());
		if(in.is_open()){ 
			in.seekg(0,ios::end);  
			str.reserve(in.tellg());
			in.seekg(0,ios::beg);
			 
			str.assign((istreambuf_iterator<char>(in)),
					istreambuf_iterator<char>());
		}
		in.close();
		
		delim = "http://espn.go.com/mens-college-basketball/bpi/_/teamId";
		pos = str.find(delim);
		str.erase(0,pos);

		allBpis bpis(str);
		//bpis.printBpis();
		bpisVector = bpis.getVector();
		for(i=0;i<bpis.getSize();i++){		
			theTeams.push_back(bpisVector[i].getTeam());
			theBpis.push_back(bpisVector[i].getBpi());
		}
	}
	//NOTE: NOT ALL TEAMS WE HAVE GAME DATA FOR ARE ON THE BPI LIST.//
/*
###############################################################################################################
###																										   ####
###						NOW WE HAVE THE BPIS OF EACH TEAM IN BPISVECTOR									   ####
###																										   ####
###############################################################################################################
*/

	//=============================GET ALL OF THE GAME HTML DATA==================================//
	if(getGameData=="1"){
		action = "python getAllGames.py";
		system(action.c_str());//This call populates websitedata.txt
		
		//SO NOW ALL OF THE GAME HTMLS ARE IN websitedata.txt
		in.open(gamedata.c_str());
		if(in.is_open()){ 
			in.seekg(0,ios::end);  
			str.reserve(in.tellg());
			in.seekg(0,ios::beg);
			 
			str.assign((istreambuf_iterator<char>(in)),
					istreambuf_iterator<char>());
		}
		in.close();
		

		//=============================PARSE ALL OF THE GAME HTML DATA==============================//
		cout << "Parsing website html data...."<<endl;
		while(str.length()>0){//take multiple game data and put all of the pageparser objects into vector.
			delim1 = "</html>";
			pos=str.find(delim1);
			temp=str.substr(0,pos+delim1.length());	
			str.erase(0,pos+delim1.length());
			pageParser current(temp);
			each_Game.push_back(current);
		}
	
	//=======================NOW ALL THE GAME DATA IS IN THE VECTOR==========================//
	
	//===============PRINTING ALL OF THE GAMES OUT TO FILE MASSGAMEDATA.TXT===================//

		out3 << "2Pt_Made_Attempted "<<"3Pt_Made_Attempted " <<"Free_Throw_Made_Attempted "<< "oreb "<<"dreb "<<"total_rebound "<<"assist "<<"steal "<<"block " << "turnovers "<<"fouls "<<"points "<<"field_goal% "<<"3pt% "<<"free_throw% "<<"home/away "<< "teamname "<< "win/loss "<< "bpi "<<"offense_value "<<"defense_value "<<endl;
		out3.close();
		for(i=0;i<each_Game.size();i++){
			temp1 = each_Game[i].getTeam1();
			temp2 = each_Game[i].getTeam2();
			t1Name = temp1.getName();
			t2Name = temp2.getName();
			
			for(j=0;j<theTeams.size();j++){
				if(theTeams[j]==t1Name){
					t1pos = j;
				}
			}
			for(k=0;k<theTeams.size();k++){
				if(theTeams[k]==t2Name){
					t2pos = k;
				}
			}
			//cout << t1Name << "~~~" << t2Name<<endl;
			each_Game[i].setT1bpi(theBpis[t1pos]);
			each_Game[i].setT2bpi(theBpis[t2pos]);
			each_Game[i].parseData();

		}
	}
	//====================Printing out team_name.txt files=============================//
/*
###############################################################################################################
###																										   ####
###																										   ####
###																										   ####
###############################################################################################################
*/
	if(getTeamGames=="1"){//PRINT OUT EACH TEAM'S INDIVIDUAL GAME DATA.. THIS IS USED TO GET AVERAGES.
		bool alreadySet;
		bool alreadySet1;
		for(i=0;i<each_Game.size();i++){
			alreadySet = false;
			alreadySet1= false;
			temp1 = each_Game[i].getTeam1();
			temp2 = each_Game[i].getTeam2();
			t1Name = temp1.getName();
			t2Name = temp2.getName();
			for(j=0;j<theTeams.size();j++){
				if(theTeams[j]==t1Name){
					t1pos = j;
				}
			}
			for(k=0;k<theTeams.size();k++){
				if(theTeams[k]==t2Name){
					t2pos = k;
				}
			}
			each_Game[i].setT1bpi(theBpis[t1pos]);
			each_Game[i].setT2bpi(theBpis[t2pos]);
			
			each_Game[i].printInfo(); 
		}
	}
/*
###############################################################################################################
###																										   ####
###																										   ####
###																										   ####
###############################################################################################################
*/
	if(getTeamAverages=="1"){//GET SEASON AVERAGES FOR EACH TEAM, AND PRINT TO MASSGAMEAVERAGE.TXT
		for(i=0; i < teamNames.size();i++){
			tempteamname = teamNames[i];
			out5 << tempteamname << endl;
			tempdelim = " ";
			size_t poss;
			poss = tempteamname.find(tempdelim);
			nameofteamfile = "";
			while(poss!=string::npos){
				nameofteamfile += tempteamname.substr(0,poss)+"_";
				tempteamname.erase(0,poss+tempdelim.length());
				poss = tempteamname.find(tempdelim);
			}

			nameofteamfile += tempteamname;
			tempOutputFileName = "teamavgs/" + nameofteamfile + "avg.txt";
			nameofteamfile = "textfiles/"+nameofteamfile+"team.txt";
			ifstream in;
			transform(nameofteamfile.begin(), nameofteamfile.end(), nameofteamfile.begin(), ::tolower);		
			transform(tempOutputFileName.begin(), tempOutputFileName.end(), tempOutputFileName.begin(), ::tolower);		

			in.open(nameofteamfile.c_str());
			if(in.is_open()){ 
				in.seekg(0,ios::end);  
				str.reserve(in.tellg());
				in.seekg(0,ios::beg);
			 
				str.assign((istreambuf_iterator<char>(in)),
				istreambuf_iterator<char>());
			}
			in.close();
			team temp(str);
			temp.printAvg(tempOutputFileName);
			teamSeasonAvgs.push_back(temp);
		}
		in.open(massgamefile.c_str());
		if(in.is_open()){ 
				in.seekg(0,ios::end);  
				str.reserve(in.tellg());
				in.seekg(0,ios::beg);
			 
				str.assign((istreambuf_iterator<char>(in)),
				istreambuf_iterator<char>());
		}
		in.close();
		delim = "\n";
		delim1 = ".";
		delim2 = "team.txt ";
		delim3 = " ";
		delim4 = "-";
		pos = str.find(delim);
		str.erase(0,pos+delim.length());
		out4 << "fgm/a 3ptm/a ftm/a oreb dreb totalreb assist steal block turnover foul points fgpercent 3ptpercent ftpercent bpi possessions pts/poss win/loss teamName OValue dValue ballControl" << endl;
		out4.close();
		cout << "Starting the average parsing." << endl;
		int avgfgm;
		int avgfga;
		int avgtpm;
		int avgtpa;
		int avgftm;
		int avgfta;
		int avgoreb;
		int avgdreb;
		int avgreb;
		int avgstl;
		int avgblk;
		int avgto;
		int avgpf;
		int avgpts;
		float avgfgpercent;
		float avg3ptpercent;
		float avgftpercent;
		float avgbpi;
		int avgpossessions;
		float avgptperposs;
		while(str.length()>0){
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());

			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());

			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());

			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());
			pos = str.find(delim3);
			str.erase(0,pos+delim3.length());

		
			//find the name of the team
			pos = str.find(delim2);
			teamPlaying = str.substr(0,pos);
			str.erase(0,pos+delim2.length());
			pos = str.find(delim3);
			winner = str.substr(0,pos);
			str.erase(0,pos);
			pos = str.find(delim);
			str.erase(0,pos+delim.length());
			//find the team average of the teamPlaying, output the average lines into MDA.txt
			averageData = "teamavgs/" + teamPlaying + "avg.txt";
			transform(averageData.begin(), averageData.end(), averageData.begin(), ::tolower);		
			in.open(averageData.c_str());
			string str1;
			if(in.is_open()){ 
				in.seekg(0,ios::end);  
				str1.reserve(in.tellg());
				in.seekg(0,ios::beg);
			 
				str1.assign((istreambuf_iterator<char>(in)),
				istreambuf_iterator<char>());
				pos = str1.find(delim);
				str1.erase(pos,pos+delim.length());
				teamavg current(str1,winner,teamPlaying);
				teamavgs.push_back(current);
			}
			in.close();
		}
		//ALL OF THE AVERAGES ARE IN teamavgs VECTOR NOW, the competitors are i vs i+1
		for(i=0;i<teamavgs.size()-1;i+=2){
			float t1Offense = (teamavgs[i+1].getturnover() + teamavgs[i].getoreb()) * teamavgs[i].getptsperpossession() + (teamavgs[i+1].getfoul() -7)*teamavgs[i].getftpercent();
			float t2Offense = (teamavgs[i].getturnover() + teamavgs[i+1].getoreb()) * teamavgs[i+1].getptsperpossession() + (teamavgs[i].getfoul() -7)*teamavgs[i+1].getftpercent();
			float t1Defense = (teamavgs[i].getblock()+teamavgs[i].getsteal() + teamavgs[i].getdreb())*teamavgs[i+1].getptsperpossession();
			float t2Defense = (teamavgs[i+1].getblock()+teamavgs[i+1].getsteal() + teamavgs[i+1].getdreb())*teamavgs[i].getptsperpossession();
			int t1BC= (teamavgs[i].getsteal()+teamavgs[i].getblock()+teamavgs[i].getreb() - teamavgs[i+1].getreb());
			int t2BC= (teamavgs[i+1].getsteal()+teamavgs[i+1].getblock()+teamavgs[i+1].getreb() - teamavgs[i].getreb());		
			out6 << teamavgs[i].getAllData()<< " " << teamavgs[i].getWinStatus()<<" " <<teamavgs[i].getName()<<" "<<t1Offense << " " << t1Defense << " " << t1BC<< " " << teamavgs[i].getpossessions() <<" " << teamavgs[i].getptsperpossession()<<" " << teamavgs[i+1].getptsperpossession()<<endl;
			out6 << teamavgs[i+1].getAllData()<< " " << teamavgs[i+1].getWinStatus()<<" " <<teamavgs[i+1].getName()<<" "<<t2Offense << " " << t2Defense << " " <<t2BC<< " "<< teamavgs[i+1].getpossessions() <<" " << teamavgs[i+1].getptsperpossession()<<" " << teamavgs[i].getptsperpossession()<<endl<<endl;
		}
	}
}
/* MAKE REVISION FOR THIS PROGRAM!
1. GET TEAM NAMES
2. MAKE TEAM OBJECT
3. EACH TEAM OBJECT HOLDS GAMES VECTOR
4. EACH TEAM OBJECT HOLDS PLAYERS VECTOR
5. EACH GAME OBJECT HOLDS PLAYER-PER-GAME STATS
6. DETERMINE INDIVIDUAL GAME STATISTICS, APPLY TO PLAYER AVERAGES
7. COMPUTE STAR POWER, CLUTCH RATING, ETC FOR EACH PLAYER
8. FIGURE OUT REGRESSION FOR EACH PLAYER STAT*/