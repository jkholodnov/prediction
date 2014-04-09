#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class team{
public:
	team(){}
	team(string line){
		size_t pos;
		int tempfgm = 0;
		int tempfga = 0;
		int temptpm = 0;
		int temptpa = 0;
		int tempftm = 0;
		int tempfta = 0;
		int temporeb = 0;
		int tempdreb = 0;
		int tempreb = 0;
		int tempast = 0;
		int tempstl = 0;
		int tempblk = 0;
		int tempto = 0;
		int temppf = 0;
		int temppts = 0;
		float tempfgpercent = 0;
		float temptppercent = 0;
		float tempftpercent = 0;
		float tempbpi = 0;
		int tempposs = 0;
		float tempppp = 0;
		string delim="\n";
		string delim1=" ";
		string delim2="-";
		int i=0;
		while(line.length()>0){			
			pos = line.find(delim2);
			tempfgm += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim2.length());
			pos = line.find(delim1);
			tempfga += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim2);
			temptpm += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim2.length());
			pos = line.find(delim1);
			temptpa += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim2);
			tempftm += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim2.length());
			pos = line.find(delim1);
			tempfta += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			temporeb += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempdreb += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempreb += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempast += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempstl += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempblk += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempto += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			temppf += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			temppts += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempfgpercent += atof(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			temptppercent += atof(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempftpercent += atof(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim1);
			tempbpi += atof(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());
		
			pos = line.find(delim1);
			tempposs += atoi(line.substr(0,pos).c_str());
			line.erase(0,pos+delim1.length());

			pos = line.find(delim);
			tempppp += atof(line.substr(0,pos).c_str());
			line.erase(0,pos+delim.length());
			i++;
			
		}
		//separate 3pts from 2pts attempts
		fgm = tempfgm /= i;
		fga = tempfga /= i;
		tpm = temptpm /= i;
		tpa = temptpa /= i;
		ftm = tempftm /= i;
		fta = tempfta /= i;
		oreb = temporeb /= i;
		dreb = tempdreb /= i;
		reb = tempreb /= i;
		ast = tempast /= i;
		stl = tempstl /= i;
		blk = tempblk /= i;
		to = tempto /= i;
		pf = temppf /= i;
		pts = temppts /= i;
		fgpercent = tempfgpercent /= i;
		tppercent = temptppercent /= i;
		ftpercent = tempftpercent /= i;
		bpi = tempbpi /= i;
		possessions = tempposs /= i;
		ptsPerPoss = tempppp /= i;
		ftpercent = ftm/fta;
	}
	//=============Print the team average to the specified filename.===============//
	void printAvg(string fileName){
		ofstream out(fileName.c_str());
		out << fgm << "-" << fga << " " << tpm << "-" << tpa << " " << ftm << "-" << fta << " " << oreb << " " << dreb << " " << reb << " " << ast << " " << stl << " " << blk << " " << to << " " << pf << " " << pts << " " << fgpercent << " " << tppercent << " " << ftpercent << " " << bpi << " " << possessions << " " << ptsPerPoss << endl; 
	}
	~team(){}
	int getfgm(){return fgm;}
	int getfga(){return fga;}
	int gettpm(){return tpm;}
	int gettpa(){return tpa;}
	int getftm(){return ftm;}
	int getfta(){return fta;}
	int getoreb(){return oreb;}
	int getdreb(){return dreb;}
	int getreb(){return reb;}
	int getast(){return ast;}
	int getstl(){return stl;}
	int getblk(){return blk;}
	int getto(){return to;}
	int getpf(){return pf;}
	int getpts(){return pts;}
	float getfgpercent(){return fgpercent;}
	float gettppercent(){return tppercent;}
	float getftpercent(){return ftpercent;}
	int getpossessions(){return possessions;}
	float getptsPerPoss(){return ptsPerPoss;}
	const team & operator = (const team & rhs){
	       	if (this == &rhs){
		    return *this;
	    	}
		else{
			fgm=rhs.fgm;
			fga=rhs.fga;
			tpm=rhs.tpm;
			tpa=rhs.tpa;
			ftm=rhs.ftm;
			fta=rhs.fta;
			oreb=rhs.oreb;
			dreb=rhs.dreb;
			reb=rhs.reb;
			ast=rhs.ast;
			stl=rhs.stl;
			blk=rhs.blk;
			to=rhs.to;
			pf=rhs.pf;
			pts=rhs.pts;
			ftpercent=rhs.ftpercent;
			tppercent=rhs.tppercent;
			fgpercent=rhs.fgpercent;
			possessions=rhs.possessions;
			ptsPerPoss = rhs.ptsPerPoss;
		}
		return *this;
	}
	bool operator == (team & rhs) const{
	       	if (
			fgm==rhs.fgm &&
			fga==rhs.fga &&
			tpm==rhs.tpm &&
			tpa==rhs.tpa &&
			ftm==rhs.ftm &&
			fta==rhs.fta &&
			oreb==rhs.oreb &&
			dreb==rhs.dreb &&
			reb==rhs.reb &&
			ast==rhs.ast &&
			stl==rhs.stl &&
			blk==rhs.blk &&
			to==rhs.to &&
			pf==rhs.pf &&
			pts==rhs.pts &&
			ftpercent==rhs.ftpercent &&
			tppercent==rhs.tppercent &&
			fgpercent==rhs.fgpercent &&
			possessions==rhs.possessions &&
			ptsPerPoss == rhs.ptsPerPoss){
			return true;		    
	    	}
		else{
			return false;
		}
	}
private:
	int fgm;
	int fga;
	int tpm;
	int tpa;
	int ftm;
	int fta;
	int oreb;
	int dreb;
	int reb;
	int ast;
	int stl;
	int blk;
	int to;
	int pf;
	int pts;
	float fgpercent;
	float tppercent;
	float ftpercent;
	int possessions;
	float ptsPerPoss;
	float bpi;
};
