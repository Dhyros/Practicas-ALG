#include <iostream>
#include <vector>

struct Punto{
    int x;
    int y;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
};

vector<Punto> DyV (vector<Punto> p, int k, int n){
    vector<Puntos> puntosNoDominados;

    for (int i=0; i<n; i++){
        bool dominado=false;

        for (int j=0; j<n; j++){
            bool mayorUno=false, mayorTodos=true;
            
            for (int a=0; a<k; a++){

                if (p[i][a] > p[j][a])
                    mayorTodos = false;

                else if (p[i][a] < p[j][a])
                    mayorUno = true;

            }

            if (mayorUno && mayorTodos){
                dominado=true;
                break;
            }
        }

        if (!dominado)
            puntosNoDominados.push (p[i]);

    }

    return puntosNoDominados;
}

using namespace std;

int main(int argc, char const **argv){

    vector<Punto> puntos;
    vector<Punto> puntosNoDominados;

    int k, n;

    cin >> k >> n;

    for (int i=0; i<n; i++){
        Punto p;
        for (int j=0; j<k; j++)
            cin >> p[j];
        puntos.add(p);
    }

    puntosNoDominados = DyV(puntos, k, n);

    for (int i=0; i<puntosNoDominados.size(); i++){
        for (int j=0; j<k; j++)
            cout << puntosNoDominados[i][j] << " ";
        cout << endl;
    }

    return 0;
}