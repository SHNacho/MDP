INC=./Eigen
SRC=./src
BIN=./bin
OBJ=./obj

OPT=-Wall -g -std=c++11

all: clean $(BIN)/main_exe

$(BIN)/main_exe :
	g++ $(SRC)/main.cpp -o $(BIN)/main_exe -I$(INC)

clean:
	rm -rf $(BIN)/*
