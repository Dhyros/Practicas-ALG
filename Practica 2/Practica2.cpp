/***************************************************************************/
// ALGORITMICA
// 
// AUTORES:
//  - Javier Montaño Rubio
//  - Hossam El Amraoui Leghzali
//  - Raul Granados Lopez
//
// Fichero con solucion a los apartados 1 y 3 del punto 3 ("Tareas a realizar").
// Utilizado para obtener los datos de los tiempos de ejecucion.
//
// Fichero: Practica2.cpp
//
// Fecha: 16/04/2023
/***************************************************************************/

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

void Fusiona1 (vector<Punto> & puntosNoDominados, const vector<Punto> & p1, const vector<Punto> & p2) {

    vector<Punto> aux = p1;
    for (int i=0; i < p2.size(); ++i)
        aux.push_back(p2[i]);

    for (int i=0; i<aux.size(); i++) {  //O(n^3)
        bool dominado=false;

        for (int j=0; j<aux.size() && !dominado; j++) {  //O(n^2)
            if (aux[j].Domina(aux[i]) && i!=j) { //O(n)
                dominado = true;
            }
        }

        if (!dominado)
            puntosNoDominados.push_back(aux[i]);

    }

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

void Fusiona2 (vector<Punto> & puntosNoDominados, const vector<Punto> & p1, const vector<Punto> & p2) {
    
    puntosNoDominados = p1;
    for (int i=0; i < p2.size(); ++i)
        puntosNoDominados.push_back(p2[i]);

    puntosNoDominados = MetodoBasico (puntosNoDominados, puntosNoDominados.size ());

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

/***************************************************************************/
/***************************************************************************/
// DEBUG (descomentar para mostrar los puntos no dominados)
// #define DEBUG

/***************************************************************************/
/***************************************************************************/

int main(int argc, char const **argv){

    if (argc != 2){
        cerr << "ERROR: Invalid number of arguments" << endl;
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return -1;
    }

    vector<Punto> puntos;
    vector<Punto> puntosNoDominadosBasico, puntosNoDominadosDyV1, puntosNoDominadosDyV2;

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

    /***********************************************************************/
    // Muestra de los resultados

    // Tamaño de la muestra (N)
    cout << n << "\t";

    // Método Básico
    time_point<steady_clock>ini = steady_clock::now();
    puntosNoDominadosBasico = MetodoBasico(puntos, n);
    time_point<steady_clock>fin = steady_clock::now();
    milliseconds_f duracion = (fin - ini);
    cout << duracion.count() << "\t";

    // Divide y vencerás 1
    ini = steady_clock::now();
    puntosNoDominadosDyV1 = DyV1(puntos, n);
    fin = steady_clock::now();
    duracion = (fin - ini);
    cout << duracion.count() << "\t";

    // Divide y vencerás 2
    ini = steady_clock::now();
    puntosNoDominadosDyV2 = DyV2(puntos, n);
    fin = steady_clock::now();
    duracion = (fin - ini);
    cout << duracion.count() << endl;

    /***********************************************************************/
    #ifdef DEBUG
    sort (puntosNoDominadosBasico.begin(), puntosNoDominadosBasico.end(), ComparaPunto);
    sort (puntosNoDominadosDyV1.begin(), puntosNoDominadosDyV1.end(), ComparaPunto);
    sort (puntosNoDominadosDyV2.begin(), puntosNoDominadosDyV2.end(), ComparaPunto);

    cout << "Puntos no dominados con método básico " << endl;

    for (int i=0; i<puntosNoDominadosBasico.size(); i++){
        for (int j=0; j<k; j++)
            cout << puntosNoDominadosBasico[i].coordenadas[j] << " ";
        cout << endl;
    }

    cout << endl;

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
    #endif

    return 0;
}
