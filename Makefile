####################################
#		  Jason Kholodnov		   #
#		Basketball Prediction	   #
####################################
CC = g++
CFLAGS = -o
LINK = g++
all: parse compare
parse: parseSeasonData.cpp
	$(CC) $(CFLAGS) parse parseSeasonData.cpp $(libs)
##
compare: compareTeams.cpp
	$(CC) $(CFLAGS) compare compareTeams.cpp $(libs)
##
clean:
	\rm compare parse
##
tar:
	tar cfv BasketballPredictor.tar Makefile compareTeams.cpp parseSeasonData.cpp