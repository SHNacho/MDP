#include <iostream>
#include <fstream>
#include "../Eigen/Dense"
#include <string>
#include <set>
#include	<chrono>

using namespace std;


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

int primerElemento_Greedy(Eigen::MatrixXf & m_distancias)
{
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	for(int i=0; i<m_distancias.rows(); ++i){

		double diversidad_i = 0;

		for(int j=0; j<m_distancias.cols(); ++j){
			diversidad_i += m_distancias(i, j);
		}

		if(diversidad_i > mayor_diversidad){
			mayor_diversidad = diversidad_i;
			mas_diverso = i;
		}
	}

	return mas_diverso;
}

// Calculamos el siguiente elemento a introducir en el vector solución
int elementoMasDiverso_Greedy(Eigen::MatrixXf & m_distancias, set<int> & v_sol){
	double mayor_diversidad = 0.0;
	int mas_diverso = 0;

	// Dentro de todos los elementos que no se encuentran en la solución
	// buscamos el que maximice la mínima distancia a uno de los elementos de la solución
	for(int i=0; i<m_distancias.rows(); ++i){

		if(v_sol.find(i) == v_sol.end()){ //Si no está ya en la solución comprobamos
			
			set<int>::iterator it = v_sol.begin();
			
			double distancia_i_sel = m_distancias(i, *it);
			it++;

			for(it; it != v_sol.end(); it++){
				if(m_distancias(i, *it) < distancia_i_sel)
					distancia_i_sel = m_distancias(i, *it);
			}

			if(distancia_i_sel > mayor_diversidad){
				mayor_diversidad = distancia_i_sel;
				mas_diverso = i;
			}
		}

	}

	return mas_diverso;
}

/**
 * 
 */
int main(int argc, char *argv[]){

	cout.setf(ios::fixed);
	int tam_solucion,
	    tam_matriz,
		 tam_actual_sol = 0;

	Eigen::MatrixXf distancias;
	set<int> solucion;

	tam_solucion = leerArchivo(argv[1], distancias);

	auto start = std::chrono::system_clock::now();
	solucion.insert(primerElemento_Greedy(distancias));
	
	while(solucion.size() < tam_solucion){
		int introducido = elementoMasDiverso_Greedy(distancias, solucion);
		solucion.insert(introducido);
	}

	auto end = std::chrono::system_clock::now();
	chrono::duration<double, milli> duration = end - start;
	
	set<int>::iterator it;

	//Muestra los elementos de la solución
	unsigned int suma = 0;
	for(it=solucion.begin(); it!=solucion.end(); it++){
		//suma += *it;
		//cout << *it << endl;
	}

	//cout << "Suma: " << suma << endl;

	//cout << "Diversidad de la solución: " << diversidad(solucion, distancias) << endl;
	//cout << "Tiempo de cálculo: " << duration.count() << " millisec" << endl;
	
	cout << diversidad(solucion, distancias) << ", " << duration.count() << endl;

	


	

	return 0;
}
