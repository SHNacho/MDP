#include "bl.h"
#include "random.h"
#include <iostream>

using namespace std;

//TODO almacenarlo en un set para que no haya repetidos
set<int> solucionInicial(Eigen::MatrixXf & m_diversidad, set<int> & solucion_inicial, int tam_solucion){
	
	int elemento;
	
	while(solucion_inicial.size() < tam_solucion){
		int elemento = Randint(0, m_diversidad.rows()-1);
		solucion_inicial.insert(elemento);
	}

	return solucion_inicial;
}

vector<ElementoConCoste> factorizarSolucion (Eigen::MatrixXf & m_diversidad, set<int> & solucion_inicial,
															vector<ElementoConCoste> & solucion_factorizada){

	set<int>::iterator it,
						    it_int;

	solucion_factorizada.resize(solucion_inicial.size(), {0, 0});

	int contador			= 0,
		 contador_interno = 0;

	for(it = solucion_inicial.begin(); it != solucion_inicial.end(); it++){
		contador_interno = contador;
		solucion_factorizada[contador].posicion = *it;

		for(it_int = it; it_int != solucion_inicial.end(); it_int++){
			solucion_factorizada[contador].diversidad += m_diversidad(*it, *it_int);
			solucion_factorizada[contador_interno].diversidad += m_diversidad(*it, *it_int);
			contador_interno++;
		}

		contador++;
	}
	
	sort(solucion_factorizada.begin(), solucion_factorizada.end());

	return solucion_factorizada;
}

double contribucion(Eigen::MatrixXf & m_diversidad, vector<ElementoConCoste> solucion,
						  int nuevo_elemento, int antiguo_elemento){

	double contribucion = 0.0;

	for(int i = 0; i < solucion.size(); ++i){
		if(i != antiguo_elemento)
			contribucion += m_diversidad(nuevo_elemento, solucion[i].posicion);
	}

	return contribucion;
}

void recalcularContribucion(Eigen::MatrixXf & m_diversidad, vector<ElementoConCoste> & solucion,
									 int nuevo_elemento, int antiguo_elemento){
	for(int i = 0; i < solucion.size(); ++i){
		if(solucion[i].posicion != nuevo_elemento){
			solucion[i].diversidad -= m_diversidad(antiguo_elemento, solucion[i].posicion);
			solucion[i].diversidad += m_diversidad(nuevo_elemento, solucion[i].posicion);
		}
	}
}

vector<ElementoConCoste> BusquedaLocal (Eigen::MatrixXf & m_diversidad, int tam_solucion){

	set<int> solucion_inicial;
	solucionInicial(m_diversidad, solucion_inicial, tam_solucion);

//	cout << "SOLUCION INICIAL" << endl;
//	for(set<int>::iterator it = solucion_inicial.begin(); it!=solucion_inicial.end(); it++){
//		cout << (*it) << endl;
//	}

	vector<ElementoConCoste> solucion;
	factorizarSolucion(m_diversidad, solucion_inicial, solucion);

//	cout << "SOLUCION FACTORIZADA" << endl;
//	for(vector<ElementoConCoste>::iterator it = solucion.begin(); it!=solucion.end(); it++){
//		cout << (*it).posicion << " - " << it->diversidad << endl;
//	}

	int iteraciones = 0;
	bool hay_mejora_vecindario = true,
		  hay_mejora_individual = false;

	vector<ElementoConCoste>::iterator it = solucion.begin();

	while(iteraciones < 100000 && hay_mejora_vecindario){

		hay_mejora_individual = false;
		while(!hay_mejora_individual && it!=solucion.end())
		{
			for(int i=0; i<m_diversidad.rows(); ++i){
				if(find(solucion.begin(), solucion.end(), i) == solucion.end()){
					double contribucion_elem = contribucion(m_diversidad, solucion, i, it->posicion);
					if(contribucion_elem > it->diversidad){
						int antiguo_elemento = it->posicion;
						ElementoConCoste nuevo_elemento = {i, contribucion_elem};
						hay_mejora_individual = true;
						*it = nuevo_elemento;
						recalcularContribucion(m_diversidad, solucion, it->posicion, antiguo_elemento);
					}
				}		
			}	
			++it;
		}

		if(hay_mejora_individual){
			sort(solucion.begin(), solucion.end());
			it=solucion.begin();
		}

		if(it == solucion.end())
			hay_mejora_vecindario = false;

		iteraciones++;
	}

	return solucion;
}




