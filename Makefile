INC=./include
SRC=./src
BIN=./bin
OBJ=./obj

OPT=-Wall -g -std=c++11

build: $(BIN)/main_exe 

all: clean build


$(BIN)/main_exe: $(OBJ)/main.o $(OBJ)/MDP.o $(OBJ)/greedy.o $(OBJ)/bl.o $(OBJ)/genetic.o $(OBJ)/random.o 
	g++ -std=c++11 -g -I$(INC) $^ -o $@ 

$(OBJ)/main.o: $(SRC)/main.cpp $(INC)/MDP.h $(INC)/greedy.h $(INC)/bl.h
	g++ -std=c++11 -g -I$(INC) -c $< -o $@ 

$(OBJ)/genetic.o: $(SRC)/genetic.cpp $(INC)/MDP.h $(INC)/bl.h $(INC)/random.h
	g++ -std=c++11  -g -I$(INC) -c $< -o $@ 

$(OBJ)/bl.o: $(SRC)/bl.cpp $(INC)/random.h $(INC)/MDP.h
	g++ -std=c++11  -g -I$(INC) -c $< -o $@ 

$(OBJ)/greedy.o: $(SRC)/greedy.cpp $(INC)/MDP.h
	g++ -std=c++11  -g -I$(INC) -c $< -o $@ 

$(OBJ)/MDP.o: $(SRC)/MDP.cpp
	g++ -std=c++11  -g -I$(INC) -c $< -o $@ 

$(OBJ)/random.o: $(SRC)/random.cpp
	g++ -std=c++11  -g  -I$(INC) -c $< -o $@ 

clean:
	rm -rf $(BIN)/* $(OBJ)/*
