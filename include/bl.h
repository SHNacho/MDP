#ifndef BL_H
#define BL_H

#include "Eigen/Dense"
#include <set>
#include <vector>

using namespace std;

/**
 * En esta estructura almacenamos un valor con su diversidad
 * Esta diversidad será la diversidad respecto al resto de elementos
 * de la solción
 */
struct ElementoConCoste{
	int posicion;
	double diversidad;

	bool operator>(const ElementoConCoste& otro) const {
		if(diversidad == otro.diversidad){
			return posicion > otro.posicion;
		}
		else
			return diversidad > otro.diversidad;
	}

	bool operator<(const ElementoConCoste& otro) const {
		if(diversidad == otro.diversidad){
			return posicion < otro.posicion;
		}
		else
			return diversidad < otro.diversidad;
	}

	bool operator==(const ElementoConCoste& otro) const {return posicion == otro.posicion;}

	bool operator==(const int otro) const {return posicion == otro;}
	ElementoConCoste & operator=(const ElementoConCoste & otro){
		this->posicion = otro.posicion;
		this->diversidad = otro.diversidad;
		return *this;
	}
};


/**
 * Calcula la solucion inicila aleatoria
 */
set<int> solucionInicial(Eigen::MatrixXf & m_diversidad, set<int> & solucion_inicial, int tam_solucion);

/**
 * Factoriza la solición inicial
 */
vector<ElementoConCoste> factorizarSolucion (Eigen::MatrixXf & m_diversidad, set<int> & solucion_inicial,
															vector<ElementoConCoste> & solucion_factorizada);


double contribucion(Eigen::MatrixXf & m_diversidad, vector<ElementoConCoste> solucion,
						  int nuevo_elemento, int antiguo_elemento);

void recalcularContribucion(Eigen::MatrixXf & m_diversidad, vector<ElementoConCoste> & solucion,
							       int nuevo_elemento, int antiguo_elemento);

vector<ElementoConCoste> BusquedaLocal (Eigen::MatrixXf & m_diversidad, int tam_solucion);

#endif
