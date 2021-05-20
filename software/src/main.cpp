#include <iostream>
#include <fstream>
#include "Eigen/Dense"
#include <string>
#include <algorithm>
#include <set>
#include	<chrono>
#include "random.h"
//#include <omp.h>

#include "MDP.h"
#include	"greedy.h"
#include "bl.h"
#include "genetic.h"

using namespace std;

//#define GREEDY
//#define GREEDY_V2
//#define LOCAL_SEARCH
#define GENETIC

const string ARCHIVO_SEMILLA = "./semilla.txt";
double mutation_prob;



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

/**
 * 
 */
int main(int argc, char *argv[]){

	//omp_set_num_threads(12);
	cout.setf(ios::fixed);

	//Leer el archivo de datos
	TARGET_SIZE = leerArchivo(argv[1]);
	
	//Establecer la semilla
	Set_random(leerSemilla());

#ifdef GREEDY 
	set<int> solucion;

	auto start = std::chrono::system_clock::now();
	MDPGreedy(TARGET_SIZE, solucion);
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;
	
	
	cout << diversidad(solucion) << ", " << duration.count() << endl;
#endif //GREEDY

#ifdef GREEDY_V2 
	set<int> solucion;

	auto start = std::chrono::system_clock::now();
	MDPGreedyV2(TARGET_SIZE, solucion);
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;
	
	
	cout << diversidad(solucion)<< ", " << duration.count() << endl;
#endif //GREEDY


#ifdef LOCAL_SEARCH
	vector<ElementoConContribucion> solucion;
	set<int> set_solucion;

	auto start = std::chrono::system_clock::now();
	solucion = BusquedaLocal(TARGET_SIZE);
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;

	for(vector<ElementoConContribucion>::iterator it = solucion.begin(); it != solucion.end(); it++){
		set_solucion.insert(it->posicion);
	}

	cout << diversidad(set_solucion) << ", " << duration.count() << endl;

#endif //LOCAL_SEARCH

#ifdef GENETIC
	mutation_prob = 0.1/(double)M_DIVERSIDAD.rows();

	auto start = std::chrono::system_clock::now();
	Individual ind = positionAGG();	
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;
	//Individual ind = uniformAGGMemetic(10, 1.0, false);	
	//Individual ind = positionAGG();	
	//Individual ind = positionAGE();	
	//Individual ind = uniformAGE();	

	//string file(argv[1]);
	cout << ind << ", " << duration.count() << endl;
#endif

	return 0;
}
