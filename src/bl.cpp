#include "bl.h"
#include "random.h"
#include "MDP.h"
#include <iostream>

using namespace std;

const int MAX_EVALUACIONES = 500000;

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


	int evaluaciones = 0;

	bool hay_mejora_vecindario		= true,
		  mejor_vecino_encontrado	= false;

	vector<ElementoConContribucion>::iterator it = solucion.begin();

	while(evaluaciones < MAX_EVALUACIONES && hay_mejora_vecindario){

		mejor_vecino_encontrado = false;
		while(!mejor_vecino_encontrado && it!=solucion.end())
		{
			for(int i=0; i<M_DIVERSIDAD.rows() && !mejor_vecino_encontrado; ++i){
				if(find(solucion.begin(), solucion.end(), i) == solucion.end()){				
					++evaluaciones;
					double contribucion_elem = contribucion(solucion, i, it->posicion);
					if(contribucion_elem > it->diversidad){
						int antiguo_elemento = it->posicion;
						ElementoConContribucion nuevo_elemento = {i, contribucion_elem};
						mejor_vecino_encontrado = true;
						*it = nuevo_elemento;
						recalcularContribucion(solucion, it->posicion, antiguo_elemento);
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




