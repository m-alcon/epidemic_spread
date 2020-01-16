CXX  = g++ -std=c++11
OBJ = ../bin/
BIN = ../bin/

SRC = $(wildcard *.cc)
TAR = $(SRC:.cc=.o)

.PHONY: all powerlaw

all: $(TAR) epidemic

epidemic: $(OBJ)epidemic.o $(OBJ)network.o
	$(CXX) -o $(BIN)epidemic.exe $^

powerlaw: $(OBJ)powerlaw.o
	$(CXX) -o $(BIN)powerlaw.exe $^
    
%.o: %.cc
	$(CXX) -c $^ -o $(OBJ)$@

clean:
	rm -rf $(OBJ)*.o $(BIN)*.exe
