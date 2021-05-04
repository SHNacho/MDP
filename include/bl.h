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
struct ElementoConContribucion{
	int posicion;
	double diversidad;

	bool operator>(const ElementoConContribucion& otro) const {
		if(diversidad == otro.diversidad){
			return posicion > otro.posicion;
		}
		else
			return diversidad > otro.diversidad;
	}

	bool operator<(const ElementoConContribucion& otro) const {
		if(diversidad == otro.diversidad){
			return posicion < otro.posicion;
		}
		else
			return diversidad < otro.diversidad;
	}

	bool operator==(const ElementoConContribucion& otro) const {return posicion == otro.posicion;}

	bool operator==(const int otro) const {return posicion == otro;}
	ElementoConContribucion & operator=(const ElementoConContribucion & otro){
		this->posicion = otro.posicion;
		this->diversidad = otro.diversidad;
		return *this;
	}
};


/**
 * Calcula la solucion inicila aleatoria
 */
set<int> solucionAleatoria(Eigen::MatrixXf & m_diversidad, set<int> & solucion_inicial, int tam_solucion);

/**
 * Factoriza la solición inicial
 */
vector<ElementoConContribucion> factorizarSolucion (Eigen::MatrixXf & m_diversidad, set<int> & solucion_inicial,
															vector<ElementoConContribucion> & solucion_factorizada);


double contribucion(Eigen::MatrixXf & m_diversidad, vector<ElementoConContribucion> solucion,
						  int nuevo_elemento, int antiguo_elemento);

void recalcularContribucion(Eigen::MatrixXf & m_diversidad, vector<ElementoConContribucion> & solucion,
							       int nuevo_elemento, int antiguo_elemento);

vector<ElementoConContribucion> BusquedaLocal (int tam_solucion);

#endif
