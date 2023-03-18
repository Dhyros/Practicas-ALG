#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

// Diseñe un algoritmo lo más eficiente posible para eliminar los elementos 
// repetidos del vector ordenado respecto al segundo vector y devuelva, como salida, el mismo vector modificado 
// con elementos sin repetir.

int* EliminaRepetidos(int* v1, int& N1, int* v2, int& N2) {

    int* v3 = new int[N1];
    int total_utilizados3 = 0;

    int i = 0;
    int j = 0;

    while (i < N1 && j < N2) {

        if (v1[i] < v2[j]) {
            v3[total_utilizados3] = v1[i];
            total_utilizados3++;
            i++;
        }
        else if (v1[i] > v2[j]) {
            j++;
        }
        else {
            i++;
            j++;
        }
    }

    while (i < N1) {
        v3[total_utilizados3] = v1[i];
        total_utilizados3++;
        i++;
    }

    N1 = total_utilizados3;

    for (int i=0; i<N1; i++) {
        v1[i] = v3[i];
    }

    delete[] v3;

    return v1;
}

// #define DEBUG

int main(int argc, char** argv) {

    /***********************************************************************/
    // Comprobación de argumentos

    if (argc != 4) {
        
        cerr << "Número de argumentos incorrecto." << endl;
        cerr << "Uso: " << argv[0] << " <N1> <N2> <semilla>" << endl;
        exit(-1);
    }

    /***********************************************************************/
    // Declaración de variables

    // Medición del tiemipo
    chrono::time_point<std::chrono::high_resolution_clock> t0, tf;

    // Vectores y semilla
    const int N1 = stoi(argv[1]);
    const int N2 = stoi(argv[2]);
    srand(stoul(argv[3]));

    int v1[N1];
    int total_utilizados1 = N1;
    int v2[N2];
    int total_utilizados2 = N2;

    // Rango de valores a introducir en el vector
    const int MIN = 0;
    const int MAX = 5;

    /***********************************************************************/
    // Rellenamos los vectores con valores aleatorios (entre 0 y 100 )

    for (int i=0; i<total_utilizados1; i++) {
        v1[i] = (rand() % (MAX-MIN)) + MIN;
    }

    for (int i=0; i<total_utilizados2; i++) {
        v2[i] = (rand() % (MAX-MIN)) + MIN;
    }

    #ifdef DEBUG
    cout << "v1 (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cout << setw(4) << v1[i];
    }
    cout << endl << endl;

    cout << "v2 (tamaño " << total_utilizados2 << "):" << endl;
    for (int i=0; i<total_utilizados2; i++) {
        cout << setw(4) << v2[i];
    }
    cout << endl << endl;
    #endif

    /***********************************************************************/
    // Medición del tiempo (en nanosegundos)

    t0 = std::chrono::high_resolution_clock::now();
    EliminaRepetidos(v1, total_utilizados1, v2, total_utilizados2);
    tf = std::chrono::high_resolution_clock::now();

    unsigned long duration;
    duration = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();

    #ifdef DEBUG
    cout << "-----------------------------------------------------------\n\n";

    cout << "v1 (tamaño " << total_utilizados1 << "):" << endl;
    for (int i=0; i<total_utilizados1; i++) {
        cout << setw(4) << v1[i];
    }
    cout << endl << endl;
    #endif

    cout << N1 << "\t" << duration << endl;


    return 0;

}
