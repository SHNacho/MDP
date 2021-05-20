#ifndef GENETIC_H
#define GENETIC_H

#include <vector>
#include "random.h"
#include "MDP.h"
#include "bl.h"

using namespace std;

extern double mutation_prob;

// Clase individuo
class Individual
{
private:
	vector<int> chromosome;
	double fitness;
	bool evaluation_required;

public:
	//Constructor aleatorio
	Individual();
	//Contructor que rellena cromosoma con el numero pasado
	Individual(int num);
	// Constructor de copia
	Individual(const Individual& ind);
	Individual(vector<int> _chromosome);
	//Funciones
	double calculateFitness();
	double genContribution(int gen_pos);
	void setEvaluationRequired(bool required);
	bool evaluationRequired();
	int& getGen(int pos);
	double getFitness();
	void repair();
	void localSearch();
	Individual positionCross(Individual &ind);
	Individual uniformCross(Individual &ind);
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

	int population_size;
	vector<Individual> population;

public:
	Population();
	Population(int _population_size);
	void add(Individual ind);
	Individual binaryTournament();
	void mutation();
	Individual& bestIndividual();
	Individual& worstIndividual();
	void evaluatePopulation();
	void sort();

	//getters
	int size();

	//Operators
	Population& operator=(const Population & pop);
	Individual& operator[](int pos);
	friend ostream& operator<<(ostream& os,  Population& ind);
};

/*********************************************************/

// Funciones necesarias
vector<int>& randomGnome(int gnome_size, int target_size);


// Algoritmos genéticos
// Generacionales
Individual uniformAGG();
Individual positionAGG();
// Estacionarios
Individual uniformAGE();
Individual positionAGE();
//
Individual uniformAGGMemetic(int num_generations, double percent_population, bool best_individuals);
#endif //GENETIC_H
