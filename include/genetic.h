#ifndef GENETIC_H
#define GENETIC_H

#include <vector>
#include "random.h"
#include "MDP.h"
#include "bl.h"

using namespace std;


// Clase individuo
class Individual
{
private:
	vector<int> chromosome;
	double fitness;
	bool evaluation_required;

public:
	//Constructor
	Individual();
	Individual(const Individual& ind);
	Individual(vector<int> _chromosome);
	//Funciones
	double calculateFitness();
	double genContribution(int gen_pos);
	void evaluationRequired(bool required);
	int& getGen(int pos);
	void repair();
	Individual& positionCross(Individual &ind);
	Individual& uniformCross(Individual &ind);
	//Sobrecarga de operadores
	bool operator <(const Individual& ind);
	Individual& operator=(const Individual& ind);
	friend ostream& operator<<(ostream& os, const Individual& ind);
};

/*********************************************************/

// Clase población
class Population
{
private:
	static double mutation_prob;

	int population_size;
	vector<Individual> population;

public:
	Population();
	Population(int _population_size);
	void add(Individual& ind);
	Individual binaryTournament();
	Population& parentsPopulation(int _population_size);
	void mutation();

	//getters
	int size();

	//Operators
	Individual& operator[](int pos);
};

/*********************************************************/

// Funciones necesarias
vector<int>& randomGnome(int gnome_size, int target_size);


//Algoritmos genéticos
void uniformAGG(Population& poblacion);
#endif //GENETIC_H
