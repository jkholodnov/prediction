import urllib2
import sys

def getAllGames(): #get each team's url page, put them into one single textfile
	num=0
	with open("textdatabases/teamurls.txt", "rU") as f:
		for line in f:#Read in each line(line contains the team urls)
			getTeamPage(line)

def getTeamPage(x): #Take the url, open and store the contents in games.txt
	input = urllib2.urlopen(str(x))
	f = open("textdatabases/games.txt",'a')
	print f
	s=input.read()
	f.write(str(s))


# you can put all of this into a class, but either way use this

# call this with:
# std::string command;
# command = "python some/path/getData.py " + arg1 + " " + arg2; std::system (command.c_str());

# when cpp calls the function above this is what is executed

if __name__ == '__main__':
    getAllGames()
