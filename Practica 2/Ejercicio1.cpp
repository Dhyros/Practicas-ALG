#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef duration<float,ratio<1,1000>> milliseconds_f ;

const int TAM = 100;

struct Punto{
    vector<int> coordenadas;

    // Devuelve si se domina a p2
    bool Domina (const Punto & p2) { //O(n)

        bool domina = true;

        for (int i=0; i<coordenadas.size() && domina; i++) { //O(n)
            if (coordenadas[i] < p2.coordenadas[i]) {
                domina = false;
            }
        }

        return domina;
    }
};

bool ComparaPunto (Punto p1, Punto p2){
    return p1.coordenadas[0] > p2.coordenadas[0];
}

vector<Punto> MetodoBasico (vector<Punto> p, int n){
    vector<Punto> puntosNoDominados;

    for (int i=0; i<n; i++) {  //O(n^3)
        bool dominado=false;

        for (int j=0; j<n && !dominado; j++) {  //O(n^2)
            if (p[j].Domina(p[i]) && i!=j) { //O(n)
                dominado = true;
            }
        }

        if (!dominado)
            puntosNoDominados.push_back(p[i]);

    }

    return puntosNoDominados;
}

vector<Punto> MetodoBasico2 (vector<Punto> p, int n){

    for (auto it1 = p.begin(); it1 != p.end(); ++it1){

        vector<Punto>::iterator it2 = p.begin();

        while (it2 != p.end()){

            if ((*it1).Domina(*it2) && it1 != it2){
               
                p.erase(it2);

                if (it1 > it2)
                    --it1;

            }else
                ++it2;

        }

    }

    return p;

}

void OrdenaCoordenada (vector<pair<Punto, bool>> & p, int coord) {
    bool cambio = true;

    for (int i=0; i<p.size() && cambio; ++i) {
        cambio = false;

        for (int j=p.size()-1; j>i; --j) {
            if (p[j].first.coordenadas[coord] > p[j-1].first.coordenadas[coord]) {
                pair<Punto, bool> aux = p[j];
                p[j] = p[j-1];
                p[j-1] = aux;
                
                cambio = true;
            }
        }
    }
}

void Fusiona1 (vector<Punto> & puntosNoDominados, const vector<Punto> & p1, const vector<Punto> & p2) {

    puntosNoDominados = p1;
    for (int i=0; i < p2.size(); ++i)
        puntosNoDominados.push_back(p2[i]);

    puntosNoDominados = MetodoBasico (puntosNoDominados, puntosNoDominados.size ());

}

vector<Punto> DyV1 (vector<Punto> p, int n) {
    vector<Punto> puntosNoDominados;

    if (n == 1)
        puntosNoDominados.push_back(p[0]);
    else {
        
        vector<Punto>::const_iterator it = p.cbegin() + n/2;
        vector<Punto> p1(p.cbegin(), it);
        vector<Punto> p2(it, p.cend());

        vector<Punto> temp1 = DyV1(p1, n/2);
        vector<Punto> temp2 = DyV1(p2, n - n/2);

        Fusiona1 (puntosNoDominados, temp1, temp2);

    }

    return puntosNoDominados;
}

vector<Punto> DyV3 (vector<Punto>& puntos) {
    int n = puntos.size();

    // Caso base: si hay menos de dos puntos, se devuelve el vector de entrada tal cual
    if (n < 2) {
        return puntos;
    }

    // Se divide el vector de entrada en dos partes iguales
    vector<Punto> puntosIzq(puntos.begin(), puntos.begin() + n / 2);
    vector<Punto> puntosDer(puntos.begin() + n / 2, puntos.end());

    // Se aplican recursivamente la función a las dos partes
    puntosIzq = DyV3(puntosIzq);
    puntosDer = DyV3(puntosDer);

    // Se combinan las soluciones de las dos partes
    vector<Punto> solucion;

    int i = 0, j = 0;
    while (i < puntosIzq.size() && j < puntosDer.size()) {
        if (puntosIzq[i].Domina(puntosDer[j])) {
            solucion.push_back(puntosIzq[i]);
            i++;
        } else {
            solucion.push_back(puntosDer[j]);
            j++;
        }
    }
    while (i < puntosIzq.size()) {
        solucion.push_back(puntosIzq[i]);
        i++;
    }
    while (j < puntosDer.size()) {
        solucion.push_back(puntosDer[j]);
        j++;
    }

    // Se eliminan los puntos dominados de la solución
    vector<Punto> noDominados;
    noDominados.push_back(solucion[0]);
    for (int i = 1; i < solucion.size(); i++) {
        if (!noDominados.back().Domina(solucion[i])) {
            noDominados.push_back(solucion[i]);
        }
    }

    return noDominados;
}

void Fusiona2 (vector<Punto> & puntosNoDominados, const vector<Punto> & p1, const vector<Punto> & p2) {
    
    puntosNoDominados = p1;
    for (int i=0; i < p2.size(); ++i)
        puntosNoDominados.push_back(p2[i]);

    puntosNoDominados = MetodoBasico2 (puntosNoDominados, puntosNoDominados.size ());

/*
    vector<pair<Punto, bool>> auxiliar;

    for (int i=0; i < p1.size(); ++i){ //O(n)
        auxiliar.push_back({p1[i], false});
    }

    for (int i=0; i < p2.size(); ++i){ //O(n)
        auxiliar.push_back({p2[i], false});
    }

    for (int i=0; i<auxiliar[0].first.coordenadas.size()-1; ++i) {
        OrdenaCoordenada(auxiliar, i);
        
        int max = -1;

        //99 88
        //89 76
        //88 77
        //88 78


        for (int j=0; j<auxiliar.size(); ++j) {
            if (auxiliar[j].first.coordenadas[i+1] >= max && !auxiliar[j].second) {
                max = auxiliar[j].first.coordenadas[i+1];
                auxiliar[j].second = true;
                if (j!=0){
                    if (puntosNoDominados[/* puntosNoDominados.size() *//*j-1].coordenadas[i]==auxiliar[j].first.coordenadas[i] || puntosNoDominados[/* puntosNoDominados.size() *//*j-1].coordenadas[i+1] < max){
                        puntosNoDominados.pop_back();
                        auxiliar[j-1].second = false;
                    }
                }
                puntosNoDominados.push_back(auxiliar[j].first);
            }
        }
    }*/
}

void Fusiona3 (vector<Punto> & puntosNoDominados, vector<Punto> & p1, vector<Punto> & p2) {

    puntosNoDominados = p1;    

    for (auto it = p1.begin(); it != p1.end(); ++it){
        for (auto it2 = p2.begin(); it != p2.end(); ++it){
            if ((*it).Domina(*it2))
                p2.erase(it2);
        }
    }
    
    vector<Punto> auxiliar = p1; //O(n)
    for (int i=0; i < p2.size(); ++i){ //O(n)
        auxiliar.push_back(p2[i]);
    }

}

vector<Punto> DyV2 (vector<Punto> p, int n) {
    vector<Punto> puntosNoDominados;

    if (n == 1)
        puntosNoDominados = p;

    else {
        vector<Punto>::const_iterator it = p.cbegin() + n/2;
        vector<Punto> p1(p.cbegin(), it);
        vector<Punto> p2(it, p.cend());

        vector<Punto> temp1 = DyV2(p1, n/2);
        vector<Punto> temp2 = DyV2(p2, n - n/2);

        Fusiona2 (puntosNoDominados, temp1, temp2);
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
    vector<Punto> puntosNoDominadosBasico, puntosNoDominadosDyV1, puntosNoDominadosDyV2, puntosNoDominadosDyV3;

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

	cout << n << "\t";
    time_point<steady_clock> ini = steady_clock::now();
    puntosNoDominadosDyV1 = DyV1(puntos, n);
    time_point<steady_clock> fin = steady_clock::now();
    milliseconds_f duracion = (fin - ini);
    cout << /*"Duración DyV1: " << */duracion.count()/* << " ms" << endl*/;
	cout << "\t";
	
    ini = steady_clock::now();
    puntosNoDominadosDyV2 = DyV2(puntos, n);
    fin = steady_clock::now();
    duracion = (fin - ini);
    cout << /*"Duración DyV2: " << */duracion.count()/* << " ms" << endl*/;
	cout << "\t";
	
    // puntosNoDominadosDyV3 = DyV3(puntos);

    ini = steady_clock::now();
    puntosNoDominadosBasico = MetodoBasico(puntos, n);
    fin = steady_clock::now();
    duracion = (fin - ini);
    cout << /*"Duración Método básico: " << */duracion.count()/* << " ms" << endl*/;
	cout << "\t";
	
    ini = steady_clock::now();
    puntosNoDominadosBasico = MetodoBasico2(puntos, n);
    fin = steady_clock::now();
    duracion = (fin - ini);
    cout << /*"Duración Método básico 2: " << */duracion.count()/* << " ms"*/ << endl;

    /*sort (puntosNoDominadosDyV1.begin(), puntosNoDominadosDyV1.end(), ComparaPunto);
    // sort (puntosNoDominadosDyV2.begin(), puntosNoDominadosDyV2.end(), ComparaPunto);
    sort (puntosNoDominadosDyV3.begin(), puntosNoDominadosDyV3.end(), ComparaPunto);
    sort (puntosNoDominadosBasico.begin(), puntosNoDominadosBasico.end(), ComparaPunto);

    cout << "Puntos no dominados con DyV1 " << endl;

    for (int i=0; i<puntosNoDominadosDyV1.size(); i++){
        for (int j=0; j<k; j++)
            cout << puntosNoDominadosDyV1[i].coordenadas[j] << " ";
        cout << endl;
    }
    
    cout << endl;

    cout << "Puntos no dominados con DyV2 " << endl;

    for (int i=0; i<puntosNoDominadosDyV2.size(); i++){
        for (int j=0; j<k; j++)
            cout << puntosNoDominadosDyV2[i].coordenadas[j] << " ";
        cout << endl;
    }

    cout << endl;

    cout << "Puntos no dominados con DyV3 " << endl;

    for (int i=0; i<puntosNoDominadosDyV3.size(); i++){
        for (int j=0; j<k; j++)
            cout << puntosNoDominadosDyV3[i].coordenadas[j] << " ";
        cout << endl;
    }

    cout << endl;

    cout << "Puntos no dominados con método básico " << endl;

    for (int i=0; i<puntosNoDominadosBasico.size(); i++){
        for (int j=0; j<k; j++)
            cout << puntosNoDominadosBasico[i].coordenadas[j] << " ";
        cout << endl;
    }*/

    return 0;
}
