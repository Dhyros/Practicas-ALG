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
    cout << "v (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cout << setw(4) << v[i];
    }
    cout << endl << endl;
    #endif

    /***********************************************************************/
    // Medición del tiempo (en nanosegundos)

    t0 = std::chrono::high_resolution_clock::now();
    EliminaRepetidos(v, total_utilizados1);
    tf = std::chrono::high_resolution_clock::now();

    unsigned long duration;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

    #ifdef DEBUG
    cout << "-----------------------------------------------------------\n\n";

    cout << "v (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cout << setw(4) << v[i];
    }
    cout << endl << endl;
    #endif

    cout << N << "\t" << duration << endl;


    return 0;

}

void EliminaRepetidos(int* v, int& N) {

    int i = 0;

    while (i < N) {

        int j = 0;

        while (j < N) {

            if (v[i] == v[j]){
                for (int k = i; k < N-1; k++){
                    v[k] = v[k+1];
                    j = 0;
                    N--;
                }
            }else{
                i++;
            }

        }

    }

}
