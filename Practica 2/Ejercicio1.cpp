#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int TAM = 100;

struct Punto{
    vector<int> coordenadas;

    bool Domina (const Punto & p2) {
        
        bool domina = true;

        for (int i=0; i<coordenadas.size() && domina; i++) {
            if (coordenadas[i] < p2.coordenadas[i]) {
                domina = false;
            }
        }

        return domina;
    }
};

// Clase Matriz n dimensional

vector<Punto> MetodoBasico (vector<Punto> p, int k, int n){
    vector<Punto> puntosNoDominados;

    for (int i=0; i<n; i++) {
        bool dominado=false;

        for (int j=0; j<n && !dominado; j++) {
            if (p[j].Domina(p[i]) && i!=j) {
                dominado = true;
            }
        }

        if (!dominado)
            puntosNoDominados.push_back(p[i]);

    }

    return puntosNoDominados;
}

void Fusiona (vector<Punto> & puntosNoDominados, const vector<Punto> & p1, const vector<Punto> & p2) {

    puntosNoDominados = p1;
    
    for (int i=0; i < p2.size(); ++i){
        puntosNoDominados.push_back(p2[i]);
    }

}

vector<Punto> DyV1 (vector<Punto> p, int k, int n) {
    vector<Punto> puntosNoDominados;

    if (n <= 2) {
        for (Punto punto : MetodoBasico(p, k, n))
            puntosNoDominados.push_back(punto);
    }
    else {
        vector<Punto>::const_iterator it = p.cbegin() + n/2;
        vector<Punto> p1(p.cbegin(), it);
        vector<Punto> p2(it, p.cend());

        vector<Punto> temp1 = DyV1(p1, k, n/2);
        vector<Punto> temp2 = DyV1(p2, k, n/2);

        Fusiona (puntosNoDominados, temp1, temp2);

        puntosNoDominados = MetodoBasico(puntosNoDominados, k, puntosNoDominados.size());
    }

    return puntosNoDominados;
}

using namespace std;

int main(int argc, char const **argv){

    if (argc != 2){
        cerr << "ERROR: Invalid number of arguments" << endl;
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return -1;
    }

    vector<Punto> puntos;
    vector<Punto> puntosNoDominados;

    string filename = argv[1];

    ifstream fi(filename);

    if (!fi) {
        cout << "ERROR: File not found" << endl;
        return -1;
    }

    int k, n;

    fi >> k >> n;

    for (int i=0; i<n; i++){
        
        Punto p;
        int coord;
        for (int j=0; j<k; j++) {
            fi >> coord;
            p.coordenadas.push_back(coord);
        }
        
        puntos.push_back(p);
    }

    puntosNoDominados = DyV1(puntos, k, n);

    for (int i=0; i<puntosNoDominados.size(); i++){
        for (int j=0; j<k; j++)
            cout << puntosNoDominados[i].coordenadas[j] << " ";
        cout << endl;
    }

    return 0;
}