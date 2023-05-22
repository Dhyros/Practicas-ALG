#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Empresa {
    int num_acciones;
    int precio_accion;
    int beneficio_accion;
    int comision;
};

int Beneficio(const Empresa & empresa) {
    return empresa.beneficio_accion * empresa.precio_accion;
}

int main(int argc, char** argv) {

    ifstream fi("empresas.txt");

    if (!fi) {
        cerr << "No se pudo abrir el archivo de empresas." << endl;
        exit(1);
    }

    int N, X;
    cout << "Introduce la cantidad de Euros a gastar: ";
    cin >> X;

    fi >> N;

    vector<Empresa> empresas(N);

    for (int i=0; i<N; ++i) {
        fi >> empresas[i].num_acciones >> empresas[i].precio_accion 
           >> empresas[i].beneficio_accion >> empresas[i].comision;
    }



    return 0;
}