#ifndef GREEDY_H
#define GREEDY_H

#include "Eigen/Dense"
#include <set>

using namespace std;

/**
 * Obtiene, de la matriz de diversidad, el elemento cuya suma de 
 * diversidades con el resto de elementos de la matriz es máxima
 */
int primerElemento(Eigen::MatrixXf & m_diversidad);

/**
 * Obtiene el elemento de la matriz de diversidad que no se encuentre en el 
 * vector solución cuya mínima diversidad los elementos de la solución sea
 * máxima con respecto al resto de elementos que no están en la solución
 */
int siguienteElemento(Eigen::MatrixXf & m_diversidad, set<int> & v_solucion);
int siguienteElementoV2(Eigen::MatrixXf & m_diversidad, set<int> & v_solucion);

set<int> MDPGreedy(Eigen::MatrixXf & m_diversidad, int tam_solucion, set<int> & solucion);
set<int> MDPGreedyV2(Eigen::MatrixXf & m_diversidad, int tam_solucion, set<int> & solucion);

#endif //GREEDY_H
