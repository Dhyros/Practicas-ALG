#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct nodo {
    int id_nodo;
    vector<nodo*> conexiones;

    nodo(int id) : id_nodo(id) {}
};

typedef pair<nodo,nodo> arista;

class Grafo {
private:
    vector<nodo> nodos;
    vector<arista> aristas;

public:

    nodo & getNodo(int pos) {
        return nodos[pos];
    }

    int NumNodos() const {
        return nodos.size();
    }

    const vector<arista> & getAristas() const {
        return aristas;
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

    friend istream operator>>(istream & in, Grafo & g) {
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
    }
};

int main() {

    ifstream fi("grafo.txt");
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

    return 0;
}