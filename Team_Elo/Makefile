CC = g++
CFLAGS = -pedantic -std=c++11 -Wall -g -O3 -pipe -lsqlite3 -pthread
SEARCH_DIRS = -I/src -I/include
LIBS = -lsqlite3 -lpthread
SRC = src/
OBJ = obj/

ALL: main
	$(CC) -o Team_Elo_Executable $(OBJ)main.o $(OBJ)games_and_teams.o $(OBJ)game.o $(OBJ)team.o $(LIBS)

main: main.o games_and_teams.o game.o team.o
	$
main.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c main.cpp -o $(OBJ)main.o
games_and_teams.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)games_and_teams.cpp -o $(OBJ)games_and_teams.o
game.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)game.cpp -o $(OBJ)game.o
team.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)team.cpp -o $(OBJ)team.o