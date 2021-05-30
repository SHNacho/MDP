#include "bl.h"
#include "random.h"
#include "MDP.h"
#include <iostream>
#include <cmath>

using namespace std;

#ifdef MULTIARRANQUE
const int MAX_EVALUACIONES = 10000;
#endif
#ifdef ITERATED
const int MAX_EVALUACIONES = 10000;
#endif
#ifdef LOCAL_SEARCH
const int MAX_EVALUACIONES = 100000;
#endif
#ifdef ES
const int MAX_EVALUACIONES = 100000;
#endif

set<int> solucionAleatoria(set<int> & solucion_inicial, int tam_solucion){
	
	int elemento;
	
	while(solucion_inicial.size() < tam_solucion){
		int elemento = Randint(0, M_DIVERSIDAD.rows()-1);
		solucion_inicial.insert(elemento);
	}

	return solucion_inicial;
}

vector<ElementoConContribucion> factorizarSolucion (set<int> & solucion_inicial,
															vector<ElementoConContribucion> & solucion_factorizada){

	
	solucion_factorizada.resize(solucion_inicial.size(), {0, 0});

	// Para iterar sobre el set
	set<int>::iterator it,
							 it_int;

	// Para iterar sobre el vector
	int contador			= 0,
		 contador_interno = 0;

	for(it = solucion_inicial.begin(); it != solucion_inicial.end(); it++){
		contador_interno = contador;
		solucion_factorizada[contador].posicion = *it;

		for(it_int = it; it_int != solucion_inicial.end(); it_int++){
			solucion_factorizada[contador].diversidad += M_DIVERSIDAD(*it, *it_int);
			solucion_factorizada[contador_interno].diversidad += M_DIVERSIDAD(*it, *it_int);
			contador_interno++;
		}

		contador++;
	}
	
	sort(solucion_factorizada.begin(), solucion_factorizada.end());

	return solucion_factorizada;
}

/*************************************************************************/

vector<ElementoConContribucion> factorizarSolucion (vector<int> & solucion_inicial,
										vector<ElementoConContribucion> & solucion_factorizada){
	
	solucion_factorizada.resize(solucion_inicial.size(), {0, 0});

	// Para iterar sobre el set
	vector<int>::iterator it,
							 it_int;

	// Para iterar sobre el vector
	int contador			= 0,
		 contador_interno = 0;

	for(it = solucion_inicial.begin(); it != solucion_inicial.end(); it++){
		contador_interno = contador;
		solucion_factorizada[contador].posicion = *it;

		for(it_int = it; it_int != solucion_inicial.end(); it_int++){
			solucion_factorizada[contador].diversidad += M_DIVERSIDAD(*it, *it_int);
			solucion_factorizada[contador_interno].diversidad += M_DIVERSIDAD(*it, *it_int);
			contador_interno++;
		}

		contador++;
	}
	
	sort(solucion_factorizada.begin(), solucion_factorizada.end());

	return solucion_factorizada;
}

double contribucion(vector<ElementoConContribucion> solucion,
						  int nuevo_elemento, int antiguo_elemento){

	double contribucion = 0.0;

	for(int i = 0; i < solucion.size(); ++i){
		if(i != antiguo_elemento)
			contribucion += M_DIVERSIDAD(nuevo_elemento, solucion[i].posicion);
	}


	return contribucion;
}


void recalcularContribucion(vector<ElementoConContribucion> & solucion,
									 int nuevo_elemento, int antiguo_elemento){
	for(int i = 0; i < solucion.size(); ++i){
		if(solucion[i].posicion != nuevo_elemento){
			solucion[i].diversidad -= M_DIVERSIDAD(antiguo_elemento, solucion[i].posicion);
			solucion[i].diversidad += M_DIVERSIDAD(nuevo_elemento, solucion[i].posicion);
		}
	}
}


vector<ElementoConContribucion> BusquedaLocal (int tam_solucion){

	set<int> solucion_inicial;
	solucionAleatoria(solucion_inicial, tam_solucion);

	vector<ElementoConContribucion> solucion;
	factorizarSolucion(solucion_inicial, solucion);

	bool hay_mejora_vecindario		= true,
		  mejor_vecino_encontrado	= false;

	vector<ElementoConContribucion>::iterator it = solucion.begin();

	while(evaluaciones < MAX_EVALUACIONES && hay_mejora_vecindario){

		mejor_vecino_encontrado = false;
		while(!mejor_vecino_encontrado && it!=solucion.end())
		{
			for(int i=0; i<M_DIVERSIDAD.rows() && !mejor_vecino_encontrado; ++i){
				if(find(solucion.begin(), solucion.end(), i) == solucion.end()){				
					double contribucion_elem = contribucion(solucion, i, it->posicion);
					if(contribucion_elem > it->diversidad){
						int antiguo_elemento = it->posicion;
						ElementoConContribucion nuevo_elemento = {i, contribucion_elem};
						mejor_vecino_encontrado = true;
						*it = nuevo_elemento;
						recalcularContribucion(solucion, it->posicion, antiguo_elemento);
						++evaluaciones;
					}
				}		
			}	
			++it;
		}

		if(mejor_vecino_encontrado){
			sort(solucion.begin(), solucion.end());
			it=solucion.begin();
		}

		if(it == solucion.end())
			hay_mejora_vecindario = false;

	}

	return solucion;
}

/**************************************************************************************/

vector<ElementoConContribucion> BusquedaLocal (vector<ElementoConContribucion> & solucion){

	bool hay_mejora_vecindario		= true,
		  mejor_vecino_encontrado	= false;

	vector<ElementoConContribucion>::iterator it = solucion.begin();

	while(evaluaciones < MAX_EVALUACIONES && hay_mejora_vecindario){

		mejor_vecino_encontrado = false;
		while(!mejor_vecino_encontrado && it!=solucion.end())
		{
			for(int i=0; i<M_DIVERSIDAD.rows() && !mejor_vecino_encontrado; ++i){
				if(find(solucion.begin(), solucion.end(), i) == solucion.end()){				
					double contribucion_elem = contribucion(solucion, i, it->posicion);
					if(contribucion_elem > it->diversidad){
						int antiguo_elemento = it->posicion;
						ElementoConContribucion nuevo_elemento = {i, contribucion_elem};
						mejor_vecino_encontrado = true;
						*it = nuevo_elemento;
						recalcularContribucion(solucion, it->posicion, antiguo_elemento);
						++evaluaciones;
					}
				}		
			}	
			++it;
		}

		if(mejor_vecino_encontrado){
			sort(solucion.begin(), solucion.end());
			it=solucion.begin();
		}

		if(it == solucion.end())
			hay_mejora_vecindario = false;

	}

	return solucion;
}
/****************************************************************/

vector<int> BusquedaLocal (vector<int> sol_inicial){


	vector<ElementoConContribucion> solucion;
	factorizarSolucion(sol_inicial, solucion);


	int evaluaciones_locales = 0;

	bool hay_mejora_vecindario		= true,
		  mejor_vecino_encontrado	= false;

	vector<ElementoConContribucion>::iterator it = solucion.begin();

	while(evaluaciones_locales < MAX_EVALUACIONES && hay_mejora_vecindario){

		mejor_vecino_encontrado = false;
		while(!mejor_vecino_encontrado && it!=solucion.end())
		{
			for(int i=0; i<M_DIVERSIDAD.rows() && !mejor_vecino_encontrado; ++i){
				if(find(solucion.begin(), solucion.end(), i) == solucion.end()){				
					double contribucion_elem = contribucion(solucion, i, it->posicion);
					if(contribucion_elem > it->diversidad){
						int antiguo_elemento = it->posicion;
						ElementoConContribucion nuevo_elemento = {i, contribucion_elem};
						mejor_vecino_encontrado = true;
						*it = nuevo_elemento;
						recalcularContribucion(solucion, it->posicion, antiguo_elemento);
					}

					++evaluaciones_locales;
					++evaluaciones;
				}		
			}	
			++it;
		}

		if(mejor_vecino_encontrado){
			sort(solucion.begin(), solucion.end());
			it=solucion.begin();
		}

		if(it == solucion.end())
			hay_mejora_vecindario = false;

	}

	// Pasamos la solución a vector de enteros
	vector<int> sol_int;

	for(int i = 0; i < solucion.size(); ++i){
		sol_int.push_back(solucion[i].posicion);
	}


	return sol_int;
}


double fitnessFact(vector<ElementoConContribucion> & sol){
	double fitness = 0.0;
	for(int i = 0; i < sol.size(); ++i){
		fitness += sol[i].diversidad;
	}

	fitness = fitness/2.0;
	return fitness;
}

vector<ElementoConContribucion> SimulatedAnnealing (){

	set<int> solucion_inicial;
	vector<ElementoConContribucion> mejor_solucion;
	solucionAleatoria(solucion_inicial, TARGET_SIZE);
	int max_vecinos = 10*TARGET_SIZE;
	int max_exitos = 0.1 * max_vecinos;
	int M = 100000/max_vecinos; // Iteracioens a realizar
	double Ci = diversidad(solucion_inicial); // Fitness solución inicial
	double Ti = (0.3 * Ci)/((-1.0)*log(0.3)); // Temperatura inicial
	double Tf = 0.001; //Temperatura final
	double T = Ti;
	int k = 1; // Número de iteraciones
	double beta = (Ti - Tf)/(M * Ti * Tf);

	vector<ElementoConContribucion> solucion_factorizada;
	factorizarSolucion(solucion_inicial, solucion_factorizada);

	mejor_solucion = solucion_factorizada;

	double Cbest = Ci; // Coste de la mejor solución
	double Cact = Ci;

	int vecinos_generados = 0;
	int exitos = 0;

	do{
		vecinos_generados = 0;
		exitos = 0;
		while(vecinos_generados < max_vecinos && exitos < max_exitos){
			// Seleccionamos un elemento aleatorio dentro del vector solucion para 
			// intercambiarlo
			int pos = Randint(0, TARGET_SIZE - 1);
			int i = solucion_factorizada[pos].posicion;
			// Seleccionamos un elemento aleatorio de fuera de la solución para 
			// intercambiarlo por el anterior
			int j;
			do{
				j = Randint(0, M_DIVERSIDAD.rows() - 1);
			}while(find(solucion_factorizada.begin(), solucion_factorizada.end(), j) != solucion_factorizada.end());
			
			// Incrementamos el número de vecinos generado
			++vecinos_generados;
			// Calculamos la diferencia de fitness entre las dos soluciones
			double contrib_j = contribucion(solucion_factorizada, j, i);
			double diferencia = solucion_factorizada[pos].diversidad - contrib_j;

			double umbral = exp(((-1.0)*diferencia/k) * T);
			double random = Randfloat(0.0, 1.0);

			// Comprobamos si se acepta la nueva solucion
			if((diferencia < 0) || (random <= umbral))
			{
				exitos++;
				ElementoConContribucion nuevo_elemento = {j, contrib_j};
				solucion_factorizada[pos] = nuevo_elemento;
				recalcularContribucion(solucion_factorizada, j, i);
				evaluaciones++;
				//Comprobamos si el coste de la solucion actual es el mejor encontrado
				Cact = fitnessFact(solucion_factorizada);

				if(Cact > Cbest){
					mejor_solucion = solucion_factorizada;
					Cbest = Cact;
				}
			}
		}
		++k;
	
		// Disminuimos la temperatura
		T = T/(1+beta*T);
		//T = 0.999*T;
		// cout << "T: " << T << " - Tf: " << Tf << endl;
		// cout << "Evaluaciones: " << evaluaciones << endl;
		// cout << "Exitos: " << exitos << endl;
	}while((T > Tf) && (evaluaciones < 100000) && (exitos > 0) );


	return mejor_solucion;
}

/***********************************BMB*******************************/

vector<ElementoConContribucion> BMB(){

	double mejor_fitness = 0;
	vector<ElementoConContribucion> mejor_solucion;

	for(int i = 0; i < 10; ++i){
		evaluaciones = 0;
		vector<ElementoConContribucion> solucion = BusquedaLocal(TARGET_SIZE);
		double fitness_act = fitnessFact(solucion);
		if(fitness_act > mejor_fitness){
			mejor_fitness = fitness_act;
			mejor_solucion = solucion;
		}
		cout << evaluaciones << endl;
	}

	return mejor_solucion;
}

/***********************************ILS*******************************/

vector<ElementoConContribucion> mutacionILS(vector<ElementoConContribucion> solucion){
	int num_mutaciones = TARGET_SIZE*0.1;
	for(int k = 0; k < num_mutaciones; ++k){
		// Seleccionamos un elemento aleatorio dentro del vector solucion para 
		// intercambiarlo
		int pos = Randint(0, TARGET_SIZE - 1);
		int i = solucion[pos].posicion;
		// Seleccionamos un elemento aleatorio de fuera de la solución para 
		// intercambiarlo por el anterior
		int j;
		do{
			j = Randint(0, M_DIVERSIDAD.rows() - 1);
		}while(find(solucion.begin(), solucion.end(), j) != solucion.end());

		double contrib_j = contribucion(solucion, j, i);
		ElementoConContribucion nuevo_elemento = {j, contrib_j};
		solucion[pos] = nuevo_elemento;
		
		recalcularContribucion(solucion, j, i);
	}

	return solucion;
}

/****************************************************************/
vector<ElementoConContribucion> ILS(){

	vector<ElementoConContribucion> mejor_solucion = BusquedaLocal(TARGET_SIZE);
	double mejor_fitness = fitnessFact(mejor_solucion);

	for(int i = 0; i < 9; ++i){
		evaluaciones = 0;
		vector<ElementoConContribucion> solucion = mutacionILS(mejor_solucion);
		solucion = BusquedaLocal(solucion);
		double fitness_act = fitnessFact(solucion);
		if(fitness_act > mejor_fitness){
			mejor_fitness = fitness_act;
			mejor_solucion = solucion;
		}
	}

	return mejor_solucion;
}










