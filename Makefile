INC=./include
SRC=./src
BIN=./bin
OBJ=./obj

OPT=-Wall -g -std=c++11

all: clean $(BIN)/main_exe

build: $(BIN)/main_exe 

$(BIN)/main_exe: $(OBJ)/main.o $(OBJ)/greedy.o $(OBJ)/bl.o $(OBJ)/random.o 
	g++ -std=c++11 -I$(INC) $^ -o $@

$(OBJ)/main.o: $(SRC)/main.cpp $(INC)/greedy.h $(INC)/bl.h
	g++ -std=c++11 -I$(INC) -c $< -o $@

$(OBJ)/bl.o: $(SRC)/bl.cpp $(INC)/random.h
	g++ -std=c++11 -I$(INC) -c $< -o $@

$(OBJ)/greedy.o: $(SRC)/greedy.cpp
	g++ -std=c++11 -I$(INC) -c $< -o $@

$(OBJ)/random.o: $(SRC)/random.cpp
	g++ -std=c++11 -I$(INC) -c $< -o $@

clean:
	rm -rf $(BIN)/* $(OBJ)/*
