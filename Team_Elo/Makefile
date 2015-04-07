CC = g++
CFLAGS = -pedantic -std=c++11 -Wall -g -O3 -pipe -lsqlite3 -pthread
#SEARCH_DIRS = -L"/usr/lib/x86_64-linux-gnu/"
SEARCH_DIRS = -I /usr/share/R/include/ -I/src -I/include -I/R/include -I/R/x86_64-pc-linux-gnu-library/3.1/Rcpp/include -I/R/x86_64-pc-linux-gnu-library/3.1/RInside/include
LIBS = -lsqlite3 -lpthread -L/usr/lib/R/lib -lR  -lblas -llapack  -L/R/x86_64-pc-linux-gnu-library/3.1/RInside/lib -lRInside -Wl,-rpath,/R/x86_64-pc-linux-gnu-library/3.1/RInside/lib

SRC = src/
OBJ = obj/

ALL: main
	$(CC) -o Team_Elo_Executable $(OBJ)main.o $(OBJ)games_and_teams.o $(OBJ)team.o $(OBJ)game.o $(OBJ)player.o $(LIBS)

main: main.o games_and_teams.o team.o game.o player.o
	$
main.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c main.cpp -o $(OBJ)main.o
games_and_teams.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)games_and_teams.cpp -o $(OBJ)games_and_teams.o
team.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)team.cpp -o $(OBJ)team.o
game.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)game.cpp -o $(OBJ)game.o
player.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)player.cpp -o $(OBJ)player.o
