#include <iostream>
#include <fstream>
#include "Eigen/Dense"
#include <string>
#include <algorithm>
#include <set>
#include <chrono>
#include "random.h"
//#include <omp.h>

#include "MDP.h"
#include	"greedy.h"
#include "bl.h"
#include "genetic.h"

using namespace std;


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
	Individual ind = uniformAGGMemetic(10, 1.0, false);	
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;
	//Individual ind = uniformAGGMemetic(10, 1.0, false);	
	//Individual ind = positionAGG();	
	//Individual ind = positionAGE();	
	//Individual ind = uniformAGE();	

	//string file(argv[1]);
	cout << ind << ", " << duration.count() << endl;
#endif

#ifdef ES 
	vector<ElementoConContribucion> solucion;
	set<int> set_solucion;
	auto start = std::chrono::system_clock::now();
	solucion = SimulatedAnnealing();
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;

	for(vector<ElementoConContribucion>::iterator it = solucion.begin(); it != solucion.end(); it++){
		set_solucion.insert(it->posicion);
	}

	cout << diversidad(set_solucion) << ", " << duration.count() << endl;
#endif
#ifdef MULTIARRANQUE
	vector<ElementoConContribucion> solucion;
	set<int> set_solucion;
	auto start = std::chrono::system_clock::now();
	solucion = BMB();
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;

	for(vector<ElementoConContribucion>::iterator it = solucion.begin(); it != solucion.end(); it++){
		set_solucion.insert(it->posicion);
	}

	cout << diversidad(set_solucion) << ", " << duration.count() << endl;
#endif
#ifdef ITERATED
	vector<ElementoConContribucion> solucion;
	set<int> set_solucion;
	auto start = std::chrono::system_clock::now();
	solucion = ILS();
	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;

	for(vector<ElementoConContribucion>::iterator it = solucion.begin(); it != solucion.end(); it++){
		set_solucion.insert(it->posicion);
	}

	cout << diversidad(set_solucion) << ", " << duration.count() << endl;
#endif

	return 0;
}
