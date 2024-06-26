/***************************************************************************/
// ALGORITMICA
// 
// AUTORES:
//  - Javier Montaño Rubio
//  - Hossam El Amraoui Leghzali
//  - Raul Granados Lopez
//
// Fichero con solucion al ejercicio 1 del apartado 2.1. Utilizado para
// obtener los datos de los tiempos de ejecucion.
//
// Fichero: ej2.1.1.cpp
//
// Fecha: 19/03/2023
/***************************************************************************/

#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

void EliminaRepetidos(int* v, int& N);

// #define DEBUG

int main(int argc, char** argv) {

    /***********************************************************************/
    // Comprobación de argumentos

    if (argc != 3) {
        
        cerr << "Número de argumentos incorrecto." << endl;
        cerr << "Uso: " << argv[0] << " <N> <semilla>" << endl;
        exit(-1);
    }

    /***********************************************************************/
    // Declaración de variables

    // Medición del tiemipo
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf;

    // Vectores y semilla
    const int N = stoi(argv[1]);
    srand(stoul(argv[2]));

    int v[N];
    int total_utilizados1 = N;

    // Rango de valores a introducir en el vector
    const int MIN = 0;
    const int MAX = N;

    /***********************************************************************/
    // Rellenamos los vectores con valores aleatorios (entre 0 y 100 )

    for (int i=0; i<total_utilizados1; i++) {
        v[i] = (rand() % (MAX-MIN)) + MIN;
    }

    #ifdef DEBUG
    cerr << "v (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cerr << setw(4) << v[i];
    }
    cerr << endl << endl;
    #endif

    /***********************************************************************/
    // Medición del tiempo (en nanosegundos)

    t0 = std::chrono::high_resolution_clock::now();
    EliminaRepetidos(v, total_utilizados1);
    tf = std::chrono::high_resolution_clock::now();

    unsigned long duration;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

    #ifdef DEBUG
    cerr << "-----------------------------------------------------------\n\n";

    cerr << "v (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cout << setw(4) << v[i];
    }
    cerr << endl << endl;
    #endif

    /***********************************************************************/
    // Mostramos el tiempo de ejecución

    cout << N << "\t" << duration << endl;


    return 0;

}

void EliminaRepetidos(int* v, int& N) {

    int i = 0;

    for (int i = 0; i < N-1; i++) {

        int j = i+1;

        while (j < N-1) {

            if (v[i] == v[j]){
                for (int k = j; k < N-1; k++){
                    v[k] = v[k+1];
                }

                N--;

            }
            else{
                j++;
            }

        }

    }

}
