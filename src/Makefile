CXX  = g++ -std=c++11
OBJ = ../bin/
BIN = ../bin/

SRC = $(wildcard *.cc)
TAR = $(SRC:.cc=.o)

.PHONY: all

all: placement visualization

placement: $(OBJ)placement.o $(OBJ)circuit.o
	$(CXX) -o $(BIN)placement $^

visualization: $(OBJ)visualization.o $(OBJ)circuit.o
	$(CXX) -o $(BIN)visualization $^
    
%.o: %.cc
	$(CXX) -c $^ -o $(OBJ)$@

clean:
	rm -rf $(OBJ)*.o $(BIN)*
