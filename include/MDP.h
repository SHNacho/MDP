#ifndef MDP_H
#define MDP_H

#include "Eigen/Dense"
#include <set>

using namespace std;

//#define GREEDY
//#define GREEDY_V2
//#define LOCAL_SEARCH
//#define GENETIC
//#define ES
//#define MULTIARRANQUE
#define ITERATED

//Variables globales comunes a todos los problemas
extern Eigen::MatrixXf M_DIVERSIDAD;
extern int TARGET_SIZE;
extern int evaluaciones;

/**
 * Lee un archivo de datos y lo pasa a una matriz de M_DIVERSIDAD
 */
int leerArchivo (string nombreArchivo);

double diversidad(set<int> & v_sol);
#endif //MDP_H
