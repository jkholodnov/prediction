#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "team.h"

using namespace std;

class teamComparison{
public:
	teamComparison(string x, string y, double t1bpirating, double t2bpirating){
		team team1temp(x);
		team team2temp(y);

		team1 = team1temp;
		team2 = team2temp;	
		
		cout << "The team objects are made." << endl;
		team1FileName = x;
		team2FileName = y;
		size_t pos;
		pos = x.find("team.txt");
		team1Name = x.substr(0,pos);
		pos = y.find("team.txt");
		team2Name = y.substr(0,pos);

		
		t1BPI = t1bpirating;
		t2BPI = t2bpirating;
		//Ball Control has a strong correlation
		team1BallControl = team1.getstl() + team1.getblk()+team1.getreb()-team2.getreb();
		team2BallControl = team2.getstl() + team2.getblk()+team2.getreb()-team1.getreb();
		t1Offense = (team2.getto() + team1.getoreb())*team1.getptsPerPoss()+(team2.getpf()-7)*team1.getftpercent();
		t2Offense = (team1.getto() + team2.getoreb())*team2.getptsPerPoss()+(team1.getpf()-7)*team2.getftpercent();
		t1Defense = (team1.getblk()+team1.getstl()+team1.getdreb()) * team2.getptsPerPoss();
		t2Defense = (team2.getblk()+team2.getstl()+team2.getdreb()) * team1.getptsPerPoss();
	}
	~teamComparison(){}
	float team1PowerRating(){
		float assistWeighted = -1*.003*team1.getast();
		float stealWeighted = .006 * team1.getstl();
		float blockWeighted = .012 * team1.getblk();
		float turnoverWeighted = -1*.026 * team1.getto();
		float fg_pctWeighted = .335 * team1.getfgpercent();
		float ft_pctWeighted = .158  * team1.getftpercent();
		float offense_ValueWeighted = .015 * t1Offense;
		float defense_ValueWeighted = -1*.009* t1Defense;
		float ball_ControlWeighted = .013 * team1BallControl;
		return assistWeighted + stealWeighted + blockWeighted + turnoverWeighted + fg_pctWeighted + ft_pctWeighted + offense_ValueWeighted + defense_ValueWeighted + ball_ControlWeighted;
	}
	float team2PowerRating(){
		float assistWeighted = -1*.003*team2.getast();
		float stealWeighted = .006 * team2.getstl();
		float blockWeighted = .012 * team2.getblk();
		float turnoverWeighted = -1*.026 * team2.getto();
		float fg_pctWeighted = .335 * team2.getfgpercent();
		float ft_pctWeighted = .158  * team2.getftpercent();
		float offense_ValueWeighted = .015 * t2Offense;
		float defense_ValueWeighted = -1*.009* t2Defense;
		float ball_ControlWeighted = .013 * team2BallControl;
		return assistWeighted + stealWeighted + blockWeighted + turnoverWeighted + fg_pctWeighted + ft_pctWeighted + offense_ValueWeighted + defense_ValueWeighted + ball_ControlWeighted;
	}
	int predict(float x,float y){
		float team1PowerRatings = x*t1BPI;
		float team2PowerRatings = y*t2BPI;
		if(team1PowerRatings > team2PowerRatings){
			return 1;
		}
		else if(team1PowerRatings < team2PowerRatings){
			return 0;
		}
		else{
			return 2;
		}
	}
private:
	team team1;
	team team2;
	string team1FileName;
	string team2FileName;
	float team1BallControl;
	float team2BallControl;
	float t1Offense;
	float t2Offense;
	float t1Defense;
	float t2Defense;
	float t1PowerRating;
	float t2PowerRating;
	double t1BPI;
	double t2BPI;
	string team1Name;
	string team2Name;
};
