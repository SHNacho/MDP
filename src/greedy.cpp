#include	"greedy.h"

int primerElemento()
{
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	for(int i=0; i<M_DIVERSIDAD.rows(); ++i){

		double diversidad_i = 0;

		for(int j=0; j<M_DIVERSIDAD.cols(); ++j){
			diversidad_i += M_DIVERSIDAD(i, j);
		}

		if(diversidad_i > mayor_diversidad){
			mayor_diversidad = diversidad_i;
			mas_diverso = i;
		}
	}

	return mas_diverso;
}

/************************************************************************/

int siguienteElemento(set<int> & v_sol){
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	// Dentro de todos los elementos que no se encuentran en la solución
	// buscamos el que maximice la mínima distancia a uno de los elementos de la solución
	for(int i=0; i<M_DIVERSIDAD.rows(); ++i){

		if(v_sol.find(i) == v_sol.end()){ //Si no está ya en la solución comprobamos
			
			set<int>::iterator it = v_sol.begin();
			
			double distancia_i_sol = M_DIVERSIDAD(i, *it);
			it++;

			for(it; it != v_sol.end(); it++){
				if(M_DIVERSIDAD(i, *it) < distancia_i_sol)
					distancia_i_sol = M_DIVERSIDAD(i, *it);
			}

			if(distancia_i_sol > mayor_diversidad){
				mayor_diversidad = distancia_i_sol;
				mas_diverso = i;
			}
		}

	}

	return mas_diverso;
}

/************************************************************************/

int siguienteElementoV2(set<int> & v_sol){
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	// Dentro de todos los elementos que no se encuentran en la solución
	// buscamos el que maximice la mínima distancia a uno de los elementos de la solución
	for(int i=0; i<M_DIVERSIDAD.rows(); ++i){

		if(v_sol.find(i) == v_sol.end()){ //Si no está ya en la solución comprobamos
			
			double distancia_i_sol = 0;
			set<int>::iterator it;

			for(it = v_sol.begin(); it != v_sol.end(); it++){
				distancia_i_sol += M_DIVERSIDAD(i, *it);
			}

			if(distancia_i_sol > mayor_diversidad){
				mayor_diversidad = distancia_i_sol;
				mas_diverso = i;
			}
		}

	}

	return mas_diverso;

}

/************************************************************************/

set<int> MDPGreedy(int tam_solucion, set<int> & solucion){
	solucion.insert(primerElemento());
	
	while(solucion.size() < tam_solucion){
		int introducido = siguienteElemento(solucion);
		solucion.insert(introducido);
	}

	return solucion;
}

/************************************************************************/

set<int> MDPGreedyV2(int tam_solucion, set<int> & solucion){
	solucion.insert(primerElemento());
	
	while(solucion.size() < tam_solucion){
		int introducido = siguienteElementoV2(solucion);
		solucion.insert(introducido);
	}

	return solucion;
}



