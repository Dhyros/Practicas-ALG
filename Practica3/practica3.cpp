#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <list>
#include <algorithm>

using namespace std;

enum Color {BLANCO, GRIS, NEGRO};

struct nodo {
    int id_nodo;
    Color color; // Color del nodo

    nodo(int id) : id_nodo(id) {}

    bool operator == (const nodo &n) const{
        return id_nodo == n.id_nodo;
    }
};

struct arista {
    nodo first;
    nodo second;

    arista (nodo n1, nodo n2) : first(n1), second (n2) {}
    bool operator == (const arista &x) const{
        return (first == x.first && second == x.second);
    }
};

class Grafo {
private:
    list<nodo> nodos;
    list<arista> aristas;

public:

    const nodo & getNodo(int pos) const{
        auto it = nodos.begin();
        advance(it, pos);
        return *it;
    }

    nodo & getNodo(int pos) {
        auto it = nodos.begin();
        advance(it, pos);
        return *it;
    }

    int NumNodos() const {
        return nodos.size();
    }

    const list<arista> & getAristas() const {
        return aristas;
    }

    list<arista> & getAristas() {
        return aristas;
    }

    void setAristas (const list<arista> &a){
        aristas = a;
    }

    const list<nodo> & getNodos() const {
        return nodos;
    }

    list<nodo> & getNodos()  {
        return nodos;
    }

    void setNodos (const list<nodo> &n){
        nodos = n;
    }

    int NumAristas() const {
        return aristas.size();
    }

    void pushNodo(const nodo & n) {
        nodos.push_back(n);
    }

    void pushArista(const arista & a) {
        aristas.push_back(a);
    }

    bool nodoRelacionado (const nodo &n1, const nodo &n2){ // O(a)
        for (arista a : aristas){
            if (a.first.id_nodo == n1.id_nodo && a.second.id_nodo == n2.id_nodo)
                return true;
            else if (a.first.id_nodo == n2.id_nodo && a.second.id_nodo == n1.id_nodo)
                return true;
        }
        return false;
    }

    void BFS (int i){ //O(n^2*a)
        nodo &s = getNodo(i);

        for (nodo &n : nodos){ //O(n)
            n.color = BLANCO;
        }

        s.color = GRIS;

        list<nodo*> cola;
        cola.push_back(&s);
        while (!cola.empty()){ //O(n^2*a)
            nodo &u = (*cola.front());
            cola.pop_front();

            for (nodo &v : nodos){  //O(n*a)
                if (nodoRelacionado(v, u)) //O(a)
                    if (v.color == BLANCO){
                        v.color = GRIS;
                        cola.push_back(&v);
                    }
            }

            u.color = NEGRO;
        }
    }

    friend istream &operator>>(istream & in, Grafo & g) {

        int n_nodos, n_aristas;

        in >> n_nodos >> n_aristas;

        for (int n=1; n <= n_nodos; ++n){
            nodo nodo_temp(n);
            g.pushNodo(nodo_temp);
        }

        for (int a=0; a < n_aristas; ++a){
            int primero, segundo;
            in >> primero >> segundo;
            arista arista_temp (g.getNodo(primero-1), g.getNodo(segundo-1));
            g.pushArista(arista_temp);
        }

        return in;

    }
};

list<arista> aristasUnidasaNodo (nodo &n, list<arista> &aristas) { //O(a)
    list<arista> solucion;

    for (arista i : aristas) {
        if (i.first.id_nodo == n.id_nodo || i.second.id_nodo == n.id_nodo) {
            solucion.push_back(i);
        }
    }

    return solucion;
}

bool grafoSigueConexo (const Grafo &g, arista &arist){ // O(n^2*a)
    Grafo aux;
    aux.setNodos(g.getNodos());
    aux.setAristas(g.getAristas());

    aux.getAristas().remove(arist); // O(n)
    aux.BFS(0); // O(n^2*a)

    for (nodo &n : aux.getNodos()){ // O(n)
        if (n.color != NEGRO) return false;
    }

    return true;
}

list<arista> Greedy (const Grafo & g) {  
    list<arista> solucion;
    list<arista> aristas = g.getAristas();

    if (g.NumNodos() <= 1) {
        return solucion;
    }

    nodo v = g.getNodo(0);

    while (solucion.size() != g.NumAristas()) {  
        list<arista> aristasV = aristasUnidasaNodo(v, aristas); 

        if (aristasV.size() == 1) {

            aristas.remove(aristasV.front()); 

            if (aristasV.front().first.id_nodo == v.id_nodo){
                v = aristasV.front().second;
            }
            else {
                v = aristasV.front().first;
                nodo tmp = aristasV.front().first.id_nodo;
                aristasV.front().first = aristasV.front().second;
                aristasV.front().second = tmp;
            }

            solucion.push_back(aristasV.front());

        } else {

            while (!grafoSigueConexo(g, aristasV.back())) { 
                aristasV.pop_back();
            }

            aristas.remove(aristasV.back()); 

            if (aristasV.back().first.id_nodo == v.id_nodo){
                v = aristasV.back().second;
            }
            else {
                v = aristasV.back().first;
                nodo aux = aristasV.back().first.id_nodo;
                aristasV.back().first = aristasV.back().second;
                aristasV.back().second = aux;
            }

            solucion.push_back(aristasV.back());
        }
    }

    return solucion;
}


int main(int argc, char ** argv) {

    /*
        Se va a suponer que los datos del grafo de entrada son siempre correctos. Es decir, que
        el grafo de entrada es un grafo de Euler.

        El objetivo es encontrar un camino de Euler que recorra todas las aristas del grafo una sola vez.
    */

    if (argc != 2) {
        cout << "Dime el nombre del fichero con el grafo" << endl;
        return 0;
    }

    ifstream fi(argv[1]);
    if (!fi) {
        cout << "No puedo abrir el fichero." << endl;
        return 1;
    }

    Grafo g;
    fi >> g;

    list<arista> solucion = Greedy(g);

    for (auto i : solucion) {
        cout << i.first.id_nodo << " " << i.second.id_nodo << endl;
    }

    return 0;
}
