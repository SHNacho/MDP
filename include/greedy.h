#ifndef GREEDY_H
#define GREEDY_H

#include "Eigen/Dense"
#include <set>
#include "MDP.h"

using namespace std;

/**
 * Obtiene, de la matriz de diversidad, el elemento cuya suma de 
 * diversidades con el resto de elementos de la matriz es máxima
 */
int primerElemento();

/**
 * Obtiene el elemento de la matriz de diversidad que no se encuentre en el 
 * vector solución cuya mínima diversidad los elementos de la solución sea
 * máxima con respecto al resto de elementos que no están en la solución
 */
int siguienteElemento(set<int> & v_solucion);
int siguienteElementoV2(set<int> & v_solucion);

set<int> MDPGreedy(int tam_solucion, set<int> & solucion);
set<int> MDPGreedyV2(int tam_solucion, set<int> & solucion);

#endif //GREEDY_H
