#include "MDP.h"
#include <fstream>


Eigen::MatrixXf M_DIVERSIDAD;
int TARGET_SIZE;

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
