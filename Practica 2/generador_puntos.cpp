/***************************************************************************/
// Fichero que genera puntos aleatorios con coordenadas entre 0 y 99.
//
// COMPILACION:
//      g++ generador_puntos.cpp -o generador_puntos
/***************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {

    srand(time(NULL));

    int n;
    string filename = "puntos.txt";

    if (argc != 2) {
        cerr << "ERROR: Invalid number of arguments" << endl;
        cerr << "Usage: " << argv[0] << " <number of points>" << endl;
        exit(-1);
    }
    n = stoi(argv[1]);

    ofstream file(filename);
    if (!file) {
        cerr << "ERROR: Unable to open file " << filename << endl;
        exit(-1);
    }


    for (int i=0; i<n; i++) {
        file << setw(2) << rand()%100 << setw(3) << rand()%100 << setw(3) << rand()%100 << endl;
    }
}
