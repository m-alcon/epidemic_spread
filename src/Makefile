CXX  = g++ -std=c++11
OBJ = ../bin/
BIN = ../bin/

SRC = $(wildcard *.cc)
TAR = $(SRC:.cc=.o)

.PHONY: all

all: $(TAR) epidemic

epidemic: $(OBJ)epidemic.o $(OBJ)network.o
	$(CXX) -o $(BIN)epidemic.exe $^
    
%.o: %.cc
	$(CXX) -c $^ -o $(OBJ)$@

clean:
	rm -rf $(OBJ)*.o $(BIN)*.exe
