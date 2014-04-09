import urllib2
import sys

def getAllGamesHtml():
	with open("textdatabases/gameUrlsParsed.txt", "rU") as f:
		for line in f:
			getSingleGame(line)
def getSingleGame(x):
	input = urllib2.urlopen(str(x))
	f = open("textdatabases/websitedata.txt",'a')
	print f
	s=input.read()
	f.write(str(s))


# you can put all of this into a class, but either way use this

# call this with:
# std::string command;
# command = "python some/path/getData.py " + arg1 + " " + arg2; std::system (command.c_str());

# when cpp calls the function above this is what is executed

if __name__ == '__main__':
    getAllGamesHtml()
