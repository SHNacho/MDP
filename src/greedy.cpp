#include	"greedy.h"

int primerElemento(Eigen::MatrixXf & m_diversidad)
{
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	for(int i=0; i<m_diversidad.rows(); ++i){

		double diversidad_i = 0;

		for(int j=0; j<m_diversidad.cols(); ++j){
			diversidad_i += m_diversidad(i, j);
		}

		if(diversidad_i > mayor_diversidad){
			mayor_diversidad = diversidad_i;
			mas_diverso = i;
		}
	}

	return mas_diverso;
}

/************************************************************************/

int siguienteElemento(Eigen::MatrixXf & m_distancias, set<int> & v_sol){
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	// Dentro de todos los elementos que no se encuentran en la solución
	// buscamos el que maximice la mínima distancia a uno de los elementos de la solución
	for(int i=0; i<m_distancias.rows(); ++i){

		if(v_sol.find(i) == v_sol.end()){ //Si no está ya en la solución comprobamos
			
			set<int>::iterator it = v_sol.begin();
			
			double distancia_i_sol = m_distancias(i, *it);
			it++;

			for(it; it != v_sol.end(); it++){
				if(m_distancias(i, *it) < distancia_i_sol)
					distancia_i_sol = m_distancias(i, *it);
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

int siguienteElementoV2(Eigen::MatrixXf & m_distancias, set<int> & v_sol){
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	// Dentro de todos los elementos que no se encuentran en la solución
	// buscamos el que maximice la mínima distancia a uno de los elementos de la solución
	for(int i=0; i<m_distancias.rows(); ++i){

		if(v_sol.find(i) == v_sol.end()){ //Si no está ya en la solución comprobamos
			
			double distancia_i_sol = 0;
			set<int>::iterator it;

			for(it = v_sol.begin(); it != v_sol.end(); it++){
				distancia_i_sol += m_distancias(i, *it);
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

set<int> MDPGreedy(Eigen::MatrixXf & m_diversidad, int tam_solucion, set<int> & solucion){
	solucion.insert(primerElemento(m_diversidad));
	
	while(solucion.size() < tam_solucion){
		int introducido = siguienteElemento(m_diversidad, solucion);
		solucion.insert(introducido);
	}

	return solucion;
}

/************************************************************************/

set<int> MDPGreedyV2(Eigen::MatrixXf & m_diversidad, int tam_solucion, set<int> & solucion){
	solucion.insert(primerElemento(m_diversidad));
	
	while(solucion.size() < tam_solucion){
		int introducido = siguienteElementoV2(m_diversidad, solucion);
		solucion.insert(introducido);
	}

	return solucion;
}



