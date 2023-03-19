#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstring>

using namespace std;

void EliminaRepetidos(int* v, int& N);
void reestructurarRaiz (int *apo, int pos, int tamapo);
void insertarEnPos (int *apo, int pos);
void HeapSort(int *v, int n);
void MergeSort(int *v, int posIni, int posFin, int *vaux);
void fusionaMS(int *v, int posIni, int centro, int posFin, int *vaux);

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

    int v[N], vaux[N];
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
    // EliminaRepetidos(v, total_utilizados1);
    MergeSort(v, 0, total_utilizados1-1, vaux);
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

void reestructurarRaiz (int *apo, int pos, int tamapo){
	int minhijo;
	if (2*pos+1<tamapo) {
		minhijo=2*pos+1;
		if ((minhijo+1 < tamapo) && (apo[minhijo] > apo[minhijo+1])) minhijo++;
		if (apo[pos] > apo[minhijo]){
			double tmp = apo[pos];
			apo[pos] = apo[minhijo];
			apo[minhijo] = tmp;
			reestructurarRaiz(apo, minhijo, tamapo);
		}
	}
}

void insertarEnPos (int *apo, int pos){

	int idx = pos - 1;
	int padre;

	if (idx > 0){
		if (idx%2 == 0){
			padre=(idx-2)/2;
		}else{
			padre=(idx-1)/2;
	    }

        if (apo[padre] > apo[idx]){
            double tmp = apo[idx];
            apo[idx] = apo[padre];
            apo[padre] = tmp;
            insertarEnPos (apo, padre+1);
        }
    }
}

void HeapSort(int *v, int n) {
	
	int *apo=new int[n];
	int tamapo = 0;

	for (int i=0; i<n; i++){
		apo[tamapo] = v[i];
		tamapo++;
		insertarEnPos (apo, tamapo);
	}

	for (int i=0; i<n; i++){
		v[i] = apo[0];
		tamapo--;
		apo[0]=apo[tamapo];
		reestructurarRaiz(apo, 0,tamapo);
	}

	delete [] apo;

}

void MergeSort(int *v, int posIni, int posFin, int *vaux) {

    if (posIni>=posFin) return;

    int centro = (posIni+posFin)/2;

    MergeSort(v, posIni, centro, vaux);
    MergeSort(v, centro+1, posFin, vaux);
    fusionaMS(v, posIni, centro+1, posFin, vaux);
}

void fusionaMS(int *v, int posIni, int centro, int posFin, int *vaux) {

    int i= posIni;
    int j= centro;
    int k= 0;

    while (i<centro && j<=posFin) {

        if (v[i]<=v[j]) {
            vaux[k]=v[i];
            i++;
        } else {
            vaux[k]=v[j];
            j++;
        }
        k++;
    }

    while (i<centro) {
        vaux[k]=v[i];
        i++;
        k++;
    }
    while (j<=posFin) {
        vaux[k]=v[j];
        j++;
        k++;
    }

    memcpy(v+posIni, vaux, k*sizeof(int));
}
