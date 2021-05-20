#include "genetic.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>      // std::rand, std::srand
#include <ctime>        // std::time
#include <bits/stdc++.h>
//#include <omp.h>

using namespace std;


// Crea un chromosoma aleatorio
vector<int>& randomGnome(int gnome_size, int target_size){
	std::srand ( unsigned ( std::time(0) ) );
	static vector<int> chromosome;
	chromosome.resize(target_size, 1);
	chromosome.resize(gnome_size, 0);
	
	shuffle(chromosome.begin(), chromosome.end(), default_random_engine(Randint(0,999999)));

	return chromosome;
}


/////////////////////// INDIVIDUAL /////////////////////////// 

// Constructor por defecto
Individual::Individual(){

	chromosome.resize(M_DIVERSIDAD.rows(), 0);

	for(int i = 0; i < TARGET_SIZE; ++i){
		int pos = Randint(0, M_DIVERSIDAD.rows() - 1);

		while(chromosome[pos] == 1){
			pos = Randint(0, M_DIVERSIDAD.rows() - 1);
		}

		chromosome[pos] = 1;
	}

	this->fitness = calculateFitness();
}

// Constructor que rellena el cromosoma con un numero dado
Individual::Individual(int num){
	this->chromosome.resize(M_DIVERSIDAD.rows(), num);
	this->fitness = 0;
	this->evaluation_required = true;
}

// Constructor de copia
Individual::Individual(const Individual& ind){
	this->chromosome = ind.chromosome;
	this->fitness = ind.fitness;
	this->evaluation_required = ind.evaluation_required;
}

/*********************************************************/

// Constructor del individuo
Individual::Individual(vector<int> _chromosome){
	this->chromosome = _chromosome;	
	this->fitness = calculateFitness();
}

/*********************************************************/

// Calcula la calidad de un cromosoma
double Individual::calculateFitness(){
	double fit = 0;

	for(int i = 0; i < chromosome.size(); ++i){
		if(chromosome[i] == 1){
			for(int j = i + 1; j < chromosome.size(); ++j){
				if(chromosome[j] == 1){
					fit += M_DIVERSIDAD(i, j);
				}
			}
		}
	}

	this->fitness = fit;
	evaluation_required = false;

	return fit;
}

/*********************************************************/

double Individual::genContribution(int gen_pos){
	double contrib = 0.0;

	for(int i = 0; i < chromosome.size(); ++i){
		if(chromosome[i] == 1){
			contrib += M_DIVERSIDAD(i, gen_pos);
		}
	}

	return contrib;
}

/*********************************************************/
void Individual::setEvaluationRequired(bool required){
	this->evaluation_required = true;
}

/*********************************************************/
bool Individual::evaluationRequired(){return evaluation_required;}
/*********************************************************/

int& Individual::getGen(int gen_pos){
	return chromosome[gen_pos];
}

/*********************************************************/

double Individual::getFitness(){
	return fitness;
}

/*********************************************************/

void Individual::repair(){
	int num_genes = count(chromosome.begin(), chromosome.end(), 1);
	
	if(num_genes > TARGET_SIZE){
		while(num_genes > TARGET_SIZE){
			//Encontrar el que más aporta	
			double mayor_contrib = 0.0;
			int pos = 0;
			
			for(int i = 0; i < chromosome.size(); ++i){
				if(chromosome[i] == 1){
					double contribucion = genContribution(i);
					if(contribucion > mayor_contrib){
						mayor_contrib = contribucion;
						pos = i;
					}
				}
			}
			chromosome[pos] = 0;
			num_genes--;
		}
	}
	else if(num_genes < TARGET_SIZE){
		while(num_genes < TARGET_SIZE){
			//Encontrar el que más aporta	
			double mayor_contrib = 0.0;
			int pos = 0;
			
			for(int i = 0; i < chromosome.size(); ++i){
				if(chromosome[i] == 0){
					double contribucion = genContribution(i);
					if(contribucion > mayor_contrib){
						mayor_contrib = contribucion;
						pos = i;
					}
				}
			}
			chromosome[pos] = 1;
			num_genes++;
		}
	}
	
}

/*********************************************************/

void Individual::localSearch(){
	vector<int> solucion;

	for(int i = 0; i < chromosome.size(); ++i){
		if(chromosome[i] == 1)
			solucion.push_back(i);
	}

	solucion = BusquedaLocal(solucion);

	fill(chromosome.begin(), chromosome.end(), 0);

	for(int i = 0; i < solucion.size(); ++i){
		chromosome[solucion[i]] = 1;
	}
	
	setEvaluationRequired(true);
}

/*********************************************************/
Individual Individual::positionCross(Individual& ind){
	// Rellenamos el vector de -1 para saber que genes no han sido
	// cruzados
	Individual child(-1);

	for(int i = 0; i < chromosome.size(); ++i){
		if(chromosome[i] == ind.chromosome[i])
			child.chromosome[i] = chromosome[i];
	}

	// Para los genes restantes
	// Seleccionamos un padre aleatorio
	int rand = Randint(0, 1);
	Individual* padre_elegido;
	if(rand == 0)
		padre_elegido = this;
	else
		padre_elegido = &ind;

	// Guardamos los genes que no han sido cruzados
	vector<int> genes;
	for(int i = 0; i < padre_elegido->chromosome.size(); ++i){
		if(child.chromosome[i] == -1){
			genes.push_back(padre_elegido->chromosome[i]);
		}
	}

	// Ordenamos aleatoriamente esos genes
	shuffle(genes.begin(), genes.end(), std::mt19937(std::random_device()()));

	// Los introducimos en el hijo
	for(int i = 0; i < child.chromosome.size(); ++i){
		if(child.chromosome[i] == -1){
			child.chromosome[i] = genes.front();
			genes.erase(genes.begin());
		}
	}

	child.evaluation_required = true;

	return child;
}

/*********************************************************/

Individual Individual::uniformCross(Individual &ind){
	Individual child(-1);

	for(int i = 0; i < chromosome.size(); ++i){
		if(chromosome[i] == ind.chromosome[i])
			child.chromosome[i] = chromosome[i];
		else{
			int padre_elegido = Randint(0, 1);
			if(padre_elegido == 0)
				child.chromosome[i] = this->chromosome[i];
			else
				child.chromosome[i] = ind.chromosome[i];
		}
	}

	child.repair();
	child.evaluation_required = true;
	return child;
}

/*********************************************************/

bool Individual::operator<(const Individual& ind){
	return this->fitness < ind.fitness;
}

/*********************************************************/

Individual& Individual::operator=(const Individual& ind){
	this->chromosome = ind.chromosome;
	this->fitness = ind.fitness;
	this->evaluation_required = ind.evaluation_required;

	return *this;
}

/*********************************************************/

ostream& operator<<(ostream& os, const Individual& ind){
	//int suma = 0;
	//for(int i = 0; i < ind.chromosome.size(); ++i){
	//	if (ind.chromosome[i] == 1)
	//		suma++;
	//}
	//os << ind.fitness << " - " << suma;
	os << ind.fitness;
	return os;
}


/////////////////////// POPULATION ///////////////////////////

Population::Population()
{
	population_size = 0;
}

/*********************************************************/

Population::Population(int _population_size){
	population_size = _population_size;

	for(int i = 0; i < population_size; ++i){	
		Individual ind; 
		population.push_back(ind);
	}

}

/*********************************************************/

void Population::add(Individual ind){
	population.push_back(ind);
	population_size++;
}

/*********************************************************/

Individual Population::binaryTournament(){
	int pos_1 = Randint(0, population_size - 1);
	int pos_2 = Randint(0, population_size - 1);
	
	if(population[pos_1] < population[pos_2])
		return population[pos_2];
	else
		return population[pos_1];
}

/*********************************************************/

void Population::mutation(){
	int num_mutaciones = mutation_prob * population.size() * M_DIVERSIDAD.rows();	

	for(int i = 0; i < num_mutaciones; ++i){
		int cromosoma_a_mutar = Randint(0, population.size() - 1);
		int gen_a_mutar_1 = Randint(0, M_DIVERSIDAD.rows() - 1);
		int gen_a_mutar_2 = Randint(0, M_DIVERSIDAD.rows() - 1);
		while (population[cromosoma_a_mutar].getGen(gen_a_mutar_1) == population[cromosoma_a_mutar].getGen(gen_a_mutar_2)){
			gen_a_mutar_2 = Randint(0, M_DIVERSIDAD.rows() - 1);
		}

		swap(population[cromosoma_a_mutar].getGen(gen_a_mutar_1), population[cromosoma_a_mutar].getGen(gen_a_mutar_2));
		population[cromosoma_a_mutar].setEvaluationRequired(true);
	}

}

/*********************************************************/

Individual& Population::bestIndividual(){
	int pos_mejor = 0;
	double mejor_fitness = 0.0;

	for(int i = 0; i < population.size(); ++i){
		if(population[i].getFitness() > mejor_fitness){
			mejor_fitness = population[i].getFitness();
			pos_mejor = i;
		}
	}

	return population[pos_mejor];
}

/*********************************************************/

Individual& Population::worstIndividual(){
	int pos_peor = 0;
	double peor_fitness = population[0].getFitness();

	for(int i = 0; i < population.size(); ++i){
		if(population[i].getFitness() < peor_fitness){
			peor_fitness = population[i].getFitness();
			pos_peor = i;
		}
	}

	return population[pos_peor];
}
/*********************************************************/

void Population::evaluatePopulation(){
	for(int i = 0; i < population.size(); ++i){
		if(population[i].evaluationRequired()){
			population[i].calculateFitness();
			evaluaciones++;
		}
	}
}

/*********************************************************/
void Population::sort(){
	std::sort(population.begin(), population.end());
}

// Getters
/*********************************************************/
int Population::size(){return population.size();}

// Operators
/*********************************************************/
Population& Population::operator=(const Population & pop){
	this->population = pop.population;
	this->population_size = pop.population_size;

	return *this;
}

/*********************************************************/
Individual& Population::operator[](int pos){return population[pos];}

/*********************************************************/
ostream& operator<<(ostream& os, Population& pop){
	for(int i = 0; i < pop.size(); ++i)
		os << i << ": " << pop[i] << endl;
		
	return os;
}
// Algoritmos genéticos
/*********************************************************/

Individual uniformAGG(){

	Population poblacion(50);
	double problabilidad_cruce = 0.7;
	int num_cruces = problabilidad_cruce * (poblacion.size() / 2);

	while(evaluaciones < 100000){
		Population nueva_poblacion;

		//Creamos la población de padres
		for(int i = 0; i < poblacion.size(); ++i){
			Individual padre(poblacion.binaryTournament());
			nueva_poblacion.add(padre);
		}

		//Cruzamos los padres
		//#pragma omp parallel for 
		for(int i = 0; i <= num_cruces; ++i){
			// Cada pareja genera dos hijos
			int pos_padre_1 = 2*i;
			int pos_padre_2 = 2*i + 1;
			
			Individual hijo_1(nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			Individual hijo_2(nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			
			// Los hijos reemplazan a los padres
			nueva_poblacion[pos_padre_1] = hijo_1;
			nueva_poblacion[pos_padre_2] = hijo_2;
		}
		//#pragma omp barrier


		// Mutación sobre la población
		nueva_poblacion.mutation();

		// Sustituimos un individuo aleatorio de la nueva población por el mejor
		// elemento de la población anterior
		int elemento_a_sustituir = Randint(0, nueva_poblacion.size() - 1);
		nueva_poblacion[elemento_a_sustituir] = poblacion.bestIndividual();

		poblacion = nueva_poblacion;

		poblacion.evaluatePopulation();
	}


	return poblacion.bestIndividual();
}

/*********************************************************/
Individual positionAGG(){

	Population poblacion(50);
	double problabilidad_cruce = 0.7;
	int num_cruces = problabilidad_cruce * (poblacion.size() / 2);

	while(evaluaciones < 100000){
		Population nueva_poblacion;

		//Creamos la población de padres
		for(int i = 0; i < poblacion.size(); ++i){
			Individual padre = poblacion.binaryTournament();
			nueva_poblacion.add(padre);
		}
		//Cruzamos los padres
		for(int i = 0; i <= num_cruces; ++i){
			// Cada pareja genera dos hijos
			int pos_padre_1 = 2*i;
			int pos_padre_2 = 2*i + 1;

			
			Individual hijo_1 = (nueva_poblacion[pos_padre_1].positionCross(nueva_poblacion[pos_padre_2]));
			Individual hijo_2 = (nueva_poblacion[pos_padre_1].positionCross(nueva_poblacion[pos_padre_2]));
			
			// Los hijos reemplazan a los padres
			nueva_poblacion[pos_padre_1] = hijo_1;
			nueva_poblacion[pos_padre_2] = hijo_2;
		}

		// Mutación sobre la población
		nueva_poblacion.mutation();

		// Sustituimos un indiduo aleatorio de la nueva población por el mejor
		// elemento de la población anterior
		int elemento_a_sustituir = Randint(0, nueva_poblacion.size() - 1);
		nueva_poblacion[elemento_a_sustituir] = poblacion.bestIndividual();

		poblacion = nueva_poblacion;

		poblacion.evaluatePopulation();
	}

	return poblacion.bestIndividual();
}

// Algoritmos genéticos estacionarios
/*********************************************************/

Individual positionAGE(){
	Population poblacion(50);
	double problabilidad_cruce = 1;

	while(evaluaciones < 100000){
		Population nueva_poblacion;

		//Creamos la población de padres
		for(int i = 0; i < 2; ++i){
			Individual padre = poblacion.binaryTournament();
			nueva_poblacion.add(padre);
		}

		int num_cruces = problabilidad_cruce * (nueva_poblacion.size() / 2);
		//Cruzamos los padres
		for(int i = 0; i < num_cruces; ++i){
			// Cada pareja genera dos hijos
			int pos_padre_1 = 2*i;
			int pos_padre_2 = 2*i + 1;

			
			Individual hijo_1 = (nueva_poblacion[pos_padre_1].positionCross(nueva_poblacion[pos_padre_2]));
			Individual hijo_2 = (nueva_poblacion[pos_padre_1].positionCross(nueva_poblacion[pos_padre_2]));
			
			// Los hijos reemplazan a los padres
			nueva_poblacion[pos_padre_1] = hijo_1;
			nueva_poblacion[pos_padre_2] = hijo_2;
		}

		// Mutación sobre la población
		nueva_poblacion.mutation();
		
		// Evaluamos la población de hijos
		nueva_poblacion.evaluatePopulation();

//		cout << "hijos" << endl << nueva_poblacion << endl;

		// Sustituimos un indiduo aleatorio de la nueva población por el mejor
		// elemento de la población anterior
		for(int i = 0; i < nueva_poblacion.size(); ++i){
			if(poblacion.worstIndividual().getFitness() < nueva_poblacion[i].getFitness())
				poblacion.worstIndividual() = nueva_poblacion[i];
		}

//		cout << poblacion << endl;

//		cout << poblacion.worstIndividual() << endl;
	}

	return poblacion.bestIndividual();
}

/*********************************************************/
Individual uniformAGE(){
	Population poblacion(50);
	double problabilidad_cruce = 1;

	while(evaluaciones < 100000){
		Population nueva_poblacion;

		//Creamos la población de padres
		for(int i = 0; i < 2; ++i){
			Individual padre = poblacion.binaryTournament();
			nueva_poblacion.add(padre);
		}

		int num_cruces = problabilidad_cruce * (nueva_poblacion.size() / 2);
		//Cruzamos los padres
		for(int i = 0; i < num_cruces; ++i){
			// Cada pareja genera dos hijos
			int pos_padre_1 = 2*i;
			int pos_padre_2 = 2*i + 1;

			
			Individual hijo_1 = (nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			Individual hijo_2 = (nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			
			// Los hijos reemplazan a los padres
			nueva_poblacion[pos_padre_1] = hijo_1;
			nueva_poblacion[pos_padre_2] = hijo_2;
		}

		// Mutación sobre la población
		nueva_poblacion.mutation();
		
		// Evaluamos la población de hijos
		nueva_poblacion.evaluatePopulation();

		// Sustituimos un indiduo aleatorio de la nueva población por el mejor
		// elemento de la población anterior
		for(int i = 0; i < nueva_poblacion.size(); ++i){
			if(poblacion.worstIndividual().getFitness() < nueva_poblacion[i].getFitness())
				poblacion.worstIndividual() = nueva_poblacion[i];
		}

	}

	return poblacion.bestIndividual();
}

/*********************************************************/
Individual uniformAGGMemetic(int num_generations, double percent_population, bool best_individuals){
	Population poblacion(50);
	double problabilidad_cruce = 0.7;
	int num_cruces = problabilidad_cruce * (poblacion.size() / 2);

	int num_individuos_bl = percent_population * poblacion.size();

	int generaciones = 0;

	while(evaluaciones < 100000){
	
		Population nueva_poblacion;

		//Creamos la población de padres
		for(int i = 0; i < poblacion.size(); ++i){
			Individual padre(poblacion.binaryTournament());
			nueva_poblacion.add(padre);
		}

		//Cruzamos los padres
		for(int i = 0; i <= num_cruces; ++i){
			// Cada pareja genera dos hijos
			int pos_padre_1 = 2*i;
			int pos_padre_2 = 2*i + 1;
			
			Individual hijo_1(nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			Individual hijo_2(nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			
			// Los hijos reemplazan a los padres
			nueva_poblacion[pos_padre_1] = hijo_1;
			nueva_poblacion[pos_padre_2] = hijo_2;
		}


		// Mutación sobre la población
		nueva_poblacion.mutation();

		// Sustituimos un individuo aleatorio de la nueva población por el mejor
		// elemento de la población anterior
		int elemento_a_sustituir = Randint(0, nueva_poblacion.size() - 1);
		nueva_poblacion[elemento_a_sustituir] = poblacion.bestIndividual();

		poblacion = nueva_poblacion;

		poblacion.evaluatePopulation();
		
		generaciones++;

		if(generaciones % num_generations == 0){
			if(best_individuals == true){
				poblacion.sort();
			}				

			for(int i = poblacion.size() - 1; i > poblacion.size() - num_individuos_bl - 1; --i){
				poblacion[i].localSearch();
			}

			poblacion.evaluatePopulation();
		}	

		// Se ha creado una nueva generación
	}


	return poblacion.bestIndividual();

}
