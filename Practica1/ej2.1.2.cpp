#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

void EliminaRepetidos(int* v1, int& N);

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

    int v1[N];
    int total_utilizados1 = N;

    // Rango de valores a introducir en el vector
    const int MIN = 0;
    const int MAX = N;

    /***********************************************************************/
    // Rellenamos los vectores con valores aleatorios (entre 0 y 100 )

    for (int i=0; i<total_utilizados1; i++) {
        v1[i] = (rand() % (MAX-MIN)) + MIN;
    }

    #ifdef DEBUG
    cerr << "v (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cerr << setw(4) << v1[i];
    }
    cerr << endl << endl;
    #endif

    /***********************************************************************/
    // Medición del tiempo (en nanosegundos)

    t0 = std::chrono::high_resolution_clock::now();
    EliminaRepetidos(v1, total_utilizados1);
    tf = std::chrono::high_resolution_clock::now();

    unsigned long duration;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

    #ifdef DEBUG
    cerr << "-----------------------------------------------------------\n\n";

    cerr << "v (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cerr << setw(4) << v1[i];
    }
    cerr << endl << endl;
    #endif

    cout << /*N << "\t" <<*/ duration << endl;

    return 0;

}

void EliminaRepetidos(int* v, int& N) {

    int i = 0;
    while (i < N) {

        if (v[i] == v[i+1]) {

            for (int j=i; j<N-1; j++) {
                v[j] = v[j+1];
            }

            N--;
        }
        else {
            i++;
        }
    }
}
