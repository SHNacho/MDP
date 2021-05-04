#include "genetic.h"
#include <algorithm>

// Crea un chromosoma aleatorio
vector<int>& random_gnome(int gnome_size, int target_size){
	static vector<int> chromosome;
	chromosome.resize(target_size, 1);
	chromosome.resize(gnome_size, 0);
	
	random_shuffle(chromosome.begin(), chromosome.end());

	return chromosome;
}


/////////////////////// INDIVIDUAL /////////////////////////// 

// Constructor por defecto
Individual::Individual(){
	this->chromosome = random_gnome(M_DIVERSIDAD.cols(), TARGET_SIZE);
	this->fitness = calculateFitness();
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
void Individual::evaluationRequired(bool required){
	this->evaluation_required = true;
}

/*********************************************************/

int& Individual::getGen(int gen_pos){
	return chromosome[gen_pos];
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

Individual& Individual::positionCross(Individual& ind){
	static Individual child;
	// Rellenamos el vector de -1 para saber que genes no han sido
	// cruzados
	fill(child.chromosome.begin(), child.chromosome.end(), -1);

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
	random_shuffle(genes.begin(), genes.end());

	// Los introducimos en el hijo
	for(int i = 0; i < child.chromosome.size(); ++i){
		if(child.chromosome[i] == -1){
			child.chromosome[i] = genes.front();
			genes.erase(child.chromosome.begin());
		}
	}

	child.evaluation_required = true;

	return child;
}

/*********************************************************/

Individual& Individual::uniformCross(Individual &ind){
	static Individual child;

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
		population.push_back(randomGnome(M_DIVERSIDAD.rows(), TARGET_SIZE));
	}

	mutation_prob = 0.1/(50 * M_DIVERSIDAD.rows());
}

/*********************************************************/

void Population::add(Individual& ind){
	population.push_back(ind);
	population_size++;
}

/*********************************************************/

Individual Population::binaryTournament(){
	int pos_1 = Randint(0, population_size);
	int pos_2 = Randint(0, population_size);

	while(pos_1 == pos_2){
		pos_2 = Randint(0, population_size);
	}
	
	if(population[pos_1] < population[pos_2])
		return population[pos_2];
	else
		return population[pos_1];
}

/*********************************************************/

Population& Population::parentsPopulation(int _population_size){
	static Population parents;
	parents.add(binaryTournament());	
	return parents;
}

/*********************************************************/

void Population::mutation(){
	int num_mutaciones = mutation_prob * population.size() * M_DIVERSIDAD.rows();	

	for(int i = 0; i < num_mutaciones; ++i){
		int cromosoma_a_mutar = Randint(0, population.size() - 1);
		int gen_a_mutar_1 = Randint(0, M_DIVERSIDAD.rows() - 1);
		int gen_a_mutar_2 = Randint(0, M_DIVERSIDAD.rows() - 1);
		while (population[cromosoma_a_mutar].getGen(gen_a_mutar_1) == population[gen_a_mutar_2].getGen(gen_a_mutar_2)){
			gen_a_mutar_2 = Randint(0, M_DIVERSIDAD.rows() - 1);
		}

		swap(population[cromosoma_a_mutar].getGen(gen_a_mutar_1), population[cromosoma_a_mutar].getGen(gen_a_mutar_2));
		population[cromosoma_a_mutar].evaluationRequired(true);
	}

}

// getters
/*********************************************************/
int Population::size(){return population.size();}

// Operators
/*********************************************************/
Individual& Population::operator[](int pos){return population[pos];}

// Algoritmos genéticos
/*********************************************************/

void uniformAGG(Population& poblacion){
	double problabilidad_cruce = 0.7;
	int num_cruces = problabilidad_cruce * (poblacion.size() / 2);

	int evaluaciones = 0;

	while(evaluaciones < 100000){
		Population nueva_poblacion;	
		//Creamos la ponblación de padres
		for(int i = 0; i < poblacion.size(); ++i){
			Individual padre = poblacion.binaryTournament();
			nueva_poblacion.add(padre);
		}
		//Cruzamos los padres
		for(int i = 0; i < num_cruces; ++i){
			// Cada pareja genera dos hijos
			int pos_padre_1 = 2*i;
			int pos_padre_2 = 2*i + 1;
			
			Individual* hijo_1 = &(nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			Individual* hijo_2 = &(nueva_poblacion[pos_padre_1].uniformCross(nueva_poblacion[pos_padre_2]));
			

		}
	}
}
