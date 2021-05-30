#include "MDP.h"
#include <fstream>
#include <set>



Eigen::MatrixXf M_DIVERSIDAD;
int TARGET_SIZE;
int evaluaciones = 0;

int leerArchivo (string nombreArchivo)
{
	int elementos,
		 tam_solucion;

	fstream fs;
	fs.open(nombreArchivo, ios::in);

	if(fs.is_open()){
		fs >> elementos;
		fs >> tam_solucion;
		
		M_DIVERSIDAD.resize(elementos, elementos);

		int fila,
			 col;

		double distancia;

		while(!fs.eof()){
			fs >> fila;
			fs >> col;
			fs >> distancia;

			M_DIVERSIDAD(fila, col) = distancia;
			M_DIVERSIDAD(col, fila) = distancia;
		}
	}
	
	fs.close();
	
	return tam_solucion;
}

/**
 * Calcula la diversidad de la solucion
 */
double diversidad(set<int> & v_sol){
	double diversidad = 0.0;

	set<int>::iterator it;
	set<int>::iterator back = v_sol.end();
	back--;

	for(it = v_sol.begin(); it != back; it++){
		set<int>::iterator it_2 = it;
		

		for(it_2++; it_2 != v_sol.end(); ++it_2){
			diversidad += M_DIVERSIDAD(*it, *it_2);
		}
	}

	return diversidad;
}
