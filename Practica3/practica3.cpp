#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <algorithm>

using namespace std;

enum Color {BLANCO, GRIS, NEGRO};

struct nodo {
    int id_nodo;
    vector<nodo> conexiones; // numero de aristas conectadas al nodo
    Color color; // Color del nodo
    //nodo padre();
    int distancia; // Distancia a raíz del arbol


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

    nodo & getNodo(int pos) {
        return nodos[pos];
    }

    int NumNodos() const {
        return nodos.size();
    }

    const list<arista> & getAristas() const {
        return aristas;
    }

    const vector<nodo> & getNodos() const {
        return nodos;
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

    void removeArista (arista & a){
        //aristas.erase(remove(aristas.begin(), aristas.end(), a), aristas.end());
        /* list<arista> aux;
        for (arista i : aristas){
            if (i.first.id_nodo != a.first.id_nodo || i.second.id_nodo != a.second.id_nodo){
                aux.push_back(i);
            }
        }
        aristas = aux; */
        aristas.remove(a);
    }

    void BFS (int i){
        nodo s = getNodo(i);
        int num = nodos.size() +1;

        for (nodo n : nodos){ //O(n)
            n.color = BLANCO;
            n.distancia = num;
        }

        s.color = GRIS;
        s.distancia = 0;

        list<nodo> cola;
        cola.push_back(s);
        while (!cola.empty()){ //O(n^2)
            nodo u = cola.front();
            cola.pop_front();

            for (nodo v : u.conexiones){  //O(n)
                if (v.color == BLANCO){
                    v.color = GRIS;
                    v.distancia = u.distancia +1;
                    //v.padre = u;
                    cola.push_back(v);
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

/* void eliminarArista (list<arista> &ar, arista a){
    list<arista> aux;
    for (arista i : ar){
        if (i.first.id_nodo != a.first.id_nodo || i.second.id_nodo != a.second.id_nodo){
                aux.push_back(i);
            }
    }
    ar = aux;

} */

list<arista> aristasUnidasaNodo (nodo &n, list<arista> &aristas) { //O(n)
    list<arista> solucion;

    for (auto i : aristas) {
        if (i.first.id_nodo == n.id_nodo || i.second.id_nodo == n.id_nodo) {
            solucion.push_back(i);
        }
    }

    return solucion;
}

bool grafoSigueConexo (const Grafo &g, arista &arist){
    Grafo aux = g;

    aux.removeArista(arist); // O(n)
    nodo s = aux.getNodo(0);
    aux.BFS(0); // O(n^2)

    for (nodo n : aux.getNodos()){ // O(n)
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

    nodo v = g.getNodos()[0];
    while (solucion.size() != g.NumAristas()) {  //O(n^3)
        list<arista> aristasV = aristasUnidasaNodo(v, aristas);

        if (aristasV.size() == 1) {
            solucion.push_back(aristasV.front());
            //aristas.erase(find(aristas.begin(), aristas.end(), aristasV.front())); //O(n)
            //eliminarArista(aristas, aristasV[0]);
            aristas.remove(aristasV.front());

            if (aristasV.front().first.id_nodo == v.id_nodo) v = aristasV.front().second;
            else  v = aristasV.front().first;

        } else {

            int i=0;
            while (!grafoSigueConexo(g, aristasV.front())) { // O(n^2)
                aristasV.pop_front();
                ++i;
            }

            solucion.push_back(aristasV.front());
            //aristas.erase(find(aristas.begin(), aristas.end(), aristasV.front())); // O(n)
            //eliminarArista(aristas, aristasV[i]);
            aristas.remove(aristasV.front());

            if (aristasV.front().first.id_nodo == v.id_nodo) v = aristasV.front().second;
            else  v = aristasV.front().first;
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
    Grafo g;
    list<arista> aristas;

    nodo n0(0);
    nodo n1(1);
    nodo n2(2);
    nodo n3(3);
    nodo n4(4);
    nodo n5(5);

    n0.conexiones.push_back(n1); n0.conexiones.push_back(n2);
    //aristas.push_back({n0, n1}); aristas.push_back({n0, n2});
    aristas.push_back(arista(n0, n1)); aristas.push_back(arista(n0, n2));

    n1.conexiones.push_back(n0); n1.conexiones.push_back(n2); n1.conexiones.push_back(n3); n1.conexiones.push_back(n4);
    //aristas.push_back({n1, n0}); aristas.push_back({n1, n2}); aristas.push_back({n1, n3}); aristas.push_back({n1, n4});
    aristas.push_back(arista(n1, n0)); aristas.push_back(arista(n1, n2)); aristas.push_back(arista(n1, n3)); aristas.push_back(arista(n1, n4));

    n2.conexiones.push_back(n0); n2.conexiones.push_back(n1); n2.conexiones.push_back(n3); n2.conexiones.push_back(n4);
    //aristas.push_back({n2, n0}); aristas.push_back({n2, n1}); aristas.push_back({n2, n3}); aristas.push_back({n2, n4});
    aristas.push_back(arista(n2, n0)); aristas.push_back(arista(n2, n1)); aristas.push_back(arista(n2, n3)); aristas.push_back(arista(n2, n4));

    n3.conexiones.push_back(n1); n3.conexiones.push_back(n2); n3.conexiones.push_back(n4); n3.conexiones.push_back(n5);
    //aristas.push_back({n3, n1}); aristas.push_back({n3, n2}); aristas.push_back({n3, n4}); aristas.push_back({n3, n5});
    aristas.push_back(arista(n3, n1)); aristas.push_back(arista(n3, n2)); aristas.push_back(arista(n3, n4)); aristas.push_back(arista(n3, n5));

    n4.conexiones.push_back(n1); n4.conexiones.push_back(n2); n4.conexiones.push_back(n3); n4.conexiones.push_back(n5);
    //aristas.push_back({n4, n1}); aristas.push_back({n4, n2}); aristas.push_back({n4, n3}); aristas.push_back({n4, n5});
    aristas.push_back(arista(n4, n1)); aristas.push_back(arista(n4, n2)); aristas.push_back(arista(n4, n3)); aristas.push_back(arista(n4, n5));

    n5.conexiones.push_back(n3); n5.conexiones.push_back(n4);
    //aristas.push_back({n5, n3}); aristas.push_back({n5, n4});
    aristas.push_back(arista(n5, n3)); aristas.push_back(arista(n5, n4));

    g.pushNodo(n0); g.pushNodo(n1); g.pushNodo(n2); g.pushNodo(n3); g.pushNodo(n4); g.pushNodo(n5);
    for (auto i : aristas) {
        g.pushArista(i);
    }

    list<arista> solucion = Greedy(g);

    for (auto i : solucion) {
        cout << i.first.id_nodo << " " << i.second.id_nodo << endl;
    }
}
