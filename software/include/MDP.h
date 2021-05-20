#ifndef MDP_H
#define MDP_H

#include "Eigen/Dense"

using namespace std;

//Variables globales comunes a todos los problemas
extern Eigen::MatrixXf M_DIVERSIDAD;
extern int TARGET_SIZE;
extern int evaluaciones;

/**
 * Lee un archivo de datos y lo pasa a una matriz de M_DIVERSIDAD
 */
int leerArchivo (string nombreArchivo);
#endif //MDP_H
