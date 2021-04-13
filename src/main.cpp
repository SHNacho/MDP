#include <iostream>
#include <fstream>
#include "Eigen/Dense"
#include <string>
#include <algorithm>
#include <set>
#include	<chrono>
#include	"greedy.h"
#include "bl.h"
#include "random.h"

using namespace std;

//#define GREEDY
//#define GREEDY_V2
#define LOCAL_SEARCH

const string ARCHIVO_SEMILLA = "./semilla.txt";

/**
 * Lee un archivo de datos y lo pasa a una matriz de distancias
 */
int leerArchivo (string nombreArchivo, Eigen::MatrixXf & distancias)
{
	int num_distancias,
		 tam_solucion;

	fstream fs;
	fs.open(nombreArchivo, ios::in);

	if(fs.is_open()){
		fs >> num_distancias;
		fs >> tam_solucion;
		
		distancias.resize(num_distancias, num_distancias);

		int fila,
			 col;

		double distancia;

		while(!fs.eof()){
			fs >> fila;
			fs >> col;
			fs >> distancia;

			distancias(fila, col) = distancia;
			distancias(col, fila) = distancia;
		}
	}
	
	fs.close();
	
	return tam_solucion;
}

/**
 * Lee la semilla de inicialización para Random del archivo semilla.txt
 */
unsigned long leerSemilla()
{
	unsigned long semilla;
	
	ifstream ifs;

	ifs.open(ARCHIVO_SEMILLA);

	if(ifs.is_open()){
		ifs >> semilla;
	}

	ifs.close();

	return semilla;
}

/**
 * Calcula la diversidad de la solucion
 */
double diversidad(set<int> & v_sol, Eigen::MatrixXf & m_distancias){
	double diversidad = 0.0;

	set<int>::iterator it;
	set<int>::iterator back = v_sol.end();
	back--;

	for(it = v_sol.begin(); it != back; it++){
		set<int>::iterator it_2 = it;
		

		for(it_2++; it_2 != v_sol.end(); ++it_2){
			diversidad += m_distancias(*it, *it_2);
		}
	}

	return diversidad;
}

bool compare_elements(ElementoConCoste & a, ElementoConCoste & b){
	return a.diversidad < b.diversidad;
}

/**
 * 
 */
int main(int argc, char *argv[]){

	cout.setf(ios::fixed);

	int tam_solucion;
	Eigen::MatrixXf m_diversidad;

	//Leer el archivo de datos
	tam_solucion = leerArchivo(argv[1], m_diversidad);
	
	//Establecer la semilla
	Set_random(leerSemilla());

#ifdef GREEDY 
	set<int> solucion;

	auto start = std::chrono::system_clock::now();
	MDPGreedy(m_diversidad, tam_solucion, solucion);
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;
	
	
	cout << diversidad(solucion, m_diversidad) << ", " << duration.count() << endl;
#endif //GREEDY

#ifdef GREEDY_V2 
	set<int> solucion;

	auto start = std::chrono::system_clock::now();
	MDPGreedyV2(m_diversidad, tam_solucion, solucion);
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;
	
	
	cout << diversidad(solucion, m_diversidad) << ", " << duration.count() << endl;
#endif //GREEDY


#ifdef LOCAL_SEARCH
	vector<ElementoConCoste> solucion;
	set<int> set_solucion;

	auto start = std::chrono::system_clock::now();
	solucion = BusquedaLocal(m_diversidad, tam_solucion);
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;

	for(vector<ElementoConCoste>::iterator it = solucion.begin(); it != solucion.end(); it++){
		set_solucion.insert(it->posicion);
	}

	cout << diversidad(set_solucion, m_diversidad) << ", " << duration.count() << endl;

#endif //LOCAL_SEARCH

	return 0;
}
