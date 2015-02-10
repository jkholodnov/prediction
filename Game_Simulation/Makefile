CC = g++
CFLAGS = -pedantic -std=c++11 -Wall -g -O3 -pipe
SEARCH_DIRS = -I/home/jason/Dropbox/current_Projects/predict/Game_Simulation/src -I/home/jason/Dropbox/current_Projects/predict/Game_Simulation/include -I/home/jason/Dropbox/current_Projects/predict/Game_Simulation -I/usr/share/R/include -I/home/jason/R/x86_64-pc-linux-gnu-library/3.0/Rcpp/include -I/home/jason/R/x86_64-pc-linux-gnu-library/3.0/RInside/include -Iinclude
LIBS = -lsqlite3 -lpthread -L/usr/lib/R/lib -lR  -lblas -llapack  -L/home/jason/R/x86_64-pc-linux-gnu-library/3.0/RInside/lib -lRInside -Wl,-rpath,/home/jason/R/x86_64-pc-linux-gnu-library/3.0/RInside/lib
SRC = /home/jason/Dropbox/current_Projects/predict/Game_Simulation/src/
DEST = /home/jason/Dropbox/current_Projects/predict/Game_Simulation/obj/Debug/src/

ALL: main
	$(CC) -o /home/jason/Dropbox/current_Projects/predict/Game_Simulation/bin/Debug/Game_Simulation $(DEST)main.o $(DEST)player.o $(DEST)simulation.o $(DEST)statistics.o $(DEST)team.o $(LIBS)
main: main.o player.o simulation.o statistics.o team.o
	$
main.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)main.cpp -o $(DEST)main.o
player.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)player.cpp -o $(DEST)player.o
simulation.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)simulation.cpp -o $(DEST)simulation.o
statistics.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)statistics.cpp -o $(DEST)statistics.o
team.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c $(SRC)team.cpp -o $(DEST)team.o