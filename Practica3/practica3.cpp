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

//typedef pair<nodo,nodo> arista;
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
    vector<nodo> nodos;
    list<arista> aristas;

public:

    const nodo & getNodo(int pos) const{
        return nodos[pos];
    }

    nodo & getNodo(int pos) {
        return nodos[pos];
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

    const vector<nodo> & getNodos() const {
        return nodos;
    }

    vector<nodo> & getNodos()  {
        return nodos;
    }

    void setNodos (const vector<nodo> &n){
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
        nodo &s = nodos[i];

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

    /* friend istream &operator>>(istream & in, Grafo & g) {
        int n, valor;

        in >> n;

        for (int i=0; i<n; ++i) {
            g.pushNodo(nodo(i));
        }

        for (int i=0; i<n; ++i) {
            for (int j=0; j<n; ++j) {
                in >> valor;
                if (valor == 1) {
                    if (find(g.getAristas().begin(), g.getAristas().end(), {g.getNodo(i), g.getNodo(j)}) == g.getAristas().end()) {
                        g.pushArista({g.getNodo(i), g.getNodo(j)});
                        g.getNodo(i).conexiones.push_back(&g.getNodo(j));
                        g.getNodo(j).conexiones.push_back(&g.getNodo(i));
                    }
                }
            }
        }

        return in;
    } */
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

list<arista> Greedy (const Grafo & g) {  // O(n^2*a^2)
    list<arista> solucion;
    list<arista> aristas = g.getAristas();

    if (g.NumNodos() <= 1) {
        return solucion;
    }

    nodo v = g.getNodo(0);

    while (solucion.size() != g.NumAristas()) {  // O(n^2*a^2)
        list<arista> aristasV = aristasUnidasaNodo(v, aristas); // O(a)

        if (aristasV.size() == 1) {
            solucion.push_back(aristasV.front());
            aristas.remove(aristasV.front()); // O(n)

            if (aristasV.front().first.id_nodo == v.id_nodo) v = aristasV.front().second;
            else  v = aristasV.front().first;

        } else {

            while (!grafoSigueConexo(g, aristasV.back())) { // O(n^2*a)
                aristasV.pop_back();
            }

            solucion.push_back(aristasV.back());
            aristas.remove(aristasV.back()); // O(n)

            if (aristasV.back().first.id_nodo == v.id_nodo) v = aristasV.back().second;
            else  v = aristasV.back().first;
        }
    }

    return solucion;
}


int main() {

    /*
        Se va a suponer que los datos del grafo de entrada son siempre correctos. Es decir, que
        el grafo de entrada es un grafo de Euler.

        El objetivo es encontrar un camino de Euler que recorra todas las aristas del grafo una sola vez.
    */

    /* ifstream fi("grafo.txt");
    if (!fi) {
        cout << "No puedo abrir el fichero." << endl;
        return 1;
    }

    Grafo g;
    fi >> g;

    for (int i=0; i<g.NumNodos(); ++i) {
        cout << "Conexiones nodo " << i << ": ";
        for (auto j : g.getNodo(i).conexiones) {

        }
    }

    return 0; */

    /* Grafo g;
    list<arista> aristas;

    nodo n1(1);
    nodo n2(2);
    nodo n3(3);
    nodo n4(4);
    nodo n5(5);
    nodo n6(6);
    nodo n7(7);
    nodo n8(8);
    nodo n9(9);
    nodo n10(10);

    arista a1 = arista(n1, n2), a2 = arista(n1, n7), a3 = arista(n1, n6), a4 = arista(n1, n5);
    aristas.push_back(a1); aristas.push_back(a2); aristas.push_back(a3); aristas.push_back(a4);

    arista a5 = arista(n2, n7), a6 = arista(n2, n8), a7 = arista(n2, n3);
    aristas.push_back(a5); aristas.push_back(a6); aristas.push_back(a7);

    arista a8 = arista(n3, n8), a9 = arista(n3, n9), a10 = arista(n3, n4);
    aristas.push_back(a8); aristas.push_back(a9); aristas.push_back(a10);

    arista a11 = arista(n4, n9), a12 = arista(n4, n10), a13 = arista(n4, n5);
    aristas.push_back(a11); aristas.push_back(a12); aristas.push_back(a13);

    arista a14 = arista(n5, n10), a15 = arista(n5, n6);
    aristas.push_back(a14); aristas.push_back(a15);

    arista a16 = arista(n6, n10), a17 = arista(n6, n7);
    aristas.push_back(a16); aristas.push_back(a17);

    arista a18 = arista(n7, n8);
    aristas.push_back(a18);

    arista a19 = arista(n8, n9);
    aristas.push_back(a19);

    arista a20 = arista(n9, n10);
    aristas.push_back(a20);

    g.pushNodo(n1); g.pushNodo(n2); g.pushNodo(n3); g.pushNodo(n4); g.pushNodo(n5);
    g.pushNodo(n6); g.pushNodo(n7); g.pushNodo(n8); g.pushNodo(n9); g.pushNodo(n10);

    for (auto i : aristas) {
        g.pushArista(i);
    }

    list<arista> solucion = Greedy(g);

    for (auto i : solucion) {
        cout << i.first.id_nodo << " " << i.second.id_nodo << endl;
    } */

    Grafo g;
    list<arista> aristas;

    nodo n1(1);
    nodo n2(2);
    nodo n3(3);
    nodo n4(4);
    nodo n5(5);
    nodo n6(6);

    arista a1 = arista(n1, n2), a2 = arista(n1, n6), a3 = arista(n1, n4), a4 = arista(n1, n5);
    aristas.push_back(a1); aristas.push_back(a2); aristas.push_back(a3); aristas.push_back(a4);

    arista a5 = arista(n2, n6), a6 = arista(n2, n3), a7 = arista(n2, n5);
    aristas.push_back(a5); aristas.push_back(a6); aristas.push_back(a7);

    arista a8 = arista(n3, n4);
    aristas.push_back(a8);

    arista a9 = arista(n4, n5), a10 = arista(n4, n6);
    aristas.push_back(a9); aristas.push_back(a10);

    arista a11 = arista(n5, n6);
    aristas.push_back(a11);

    g.pushNodo(n1); g.pushNodo(n2); g.pushNodo(n3); g.pushNodo(n4); g.pushNodo(n5); g.pushNodo(n6);

    for (auto i : aristas) {
        g.pushArista(i);
    }

    list<arista> solucion = Greedy(g);

    for (auto i : solucion) {
        cout << i.first.id_nodo << " " << i.second.id_nodo << endl;
    }
}
