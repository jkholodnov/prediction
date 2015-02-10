CC = g++
CFLAGS = -pedantic -std=c++11 -Wall -g -O3 -pipe -lsqlite3 -pthread
SEARCH_DIRS = -I/src -I/include -I/R/include -I/R/x86_64-pc-linux-gnu-library/3.1/Rcpp/include -I/R/x86_64-pc-linux-gnu-library/3.1/RInside/include
LIBS = -lsqlite3 -lpthread -L/usr/lib/R/lib -lR  -lblas -llapack  -L/R/x86_64-pc-linux-gnu-library/3.1/RInside/lib -lRInside -Wl,-rpath,/R/x86_64-pc-linux-gnu-library/3.1/RInside/lib
SRC = src/
OBJ = obj/

ALL: main
	$(CC) -o ../Game_Simulation $(OBJ)main.o $(OBJ)player.o $(OBJ)simulation.o $(OBJ)statistics.o $(OBJ)team.o $(LIBS)
main: main.o player.o simulation.o statistics.o team.o
	$
main.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)main.cpp -o $(OBJ)main.o
player.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)player.cpp -o $(OBJ)player.o
simulation.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)simulation.cpp -o $(OBJ)simulation.o
statistics.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)statistics.cpp -o $(OBJ)statistics.o
team.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)team.cpp -o $(OBJ)team.o