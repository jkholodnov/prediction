CC = g++
CFLAGS = -pedantic -std=c++11 -Wall -g -O3 -pipe -lsqlite3 -pthread -pg
#SEARCH_DIRS = -L"/usr/lib/x86_64-linux-gnu/"
SEARCH_DIRS = -I /usr/share/R/include/ -I/src -I/include -I/R/include -I/R/x86_64-pc-linux-gnu-library/3.1/Rcpp/include -I/R/x86_64-pc-linux-gnu-library/3.1/RInside/include
LIBS = -lsqlite3 -lpthread -L/usr/lib/R/lib -lR  -lblas -llapack  -L/R/x86_64-pc-linux-gnu-library/3.1/RInside/lib -lRInside -Wl,-rpath,/R/x86_64-pc-linux-gnu-library/3.1/RInside/lib

SRC = src/
OBJ = obj/

ALL: main
	$(CC) $(CFLAGS) -o Betting $(OBJ)main.o $(LIBS)

main: main.o
	$
main.o:
	$(CC) $(CFLAGS) $(SEARCH_DIRS) -c main.cpp -o $(OBJ)main.o
