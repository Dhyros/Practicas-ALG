#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Estructura para representar una empresa
struct Empresa {
    int acciones_disponibles;
    float precio_accion;
    float beneficio;
    float comision;
};

// Función para resolver el problema utilizando programación dinámica
void resolver(int X, const vector<Empresa>& empresas) {
    int N = empresas.size();

    // Ordenar las empresas por la razón beneficio / precio en orden descendente
    vector<pair<double, int>> orden;
    for (int i = 0; i < N; i++) {
        double razon = static_cast<double>(empresas[i].beneficio) / empresas[i].precio_accion;
        orden.push_back({razon, i});
    }
    sort(orden.rbegin(), orden.rend());

    // Inicializar la matriz de programación dinámica
    vector<vector<int>> PD(N + 1, vector<int>(X + 1, 0));

    // Rellenar casos base
    for (int i = 0; i <= N; i++) {
        PD[i][0] = 0;
    }
    for (int j = 0; j <= X; j++) {
        int empresa = orden[0].second;
        PD[0][j] = empresas[empresa].beneficio *
                   min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion)) *
                   empresas[empresa].precio_accion -
                   empresas[empresa].comision * min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion));
    }

    // Calcular el beneficio máximo
    for (int i = 1; i <= N; i++) {
        int empresa = orden[i-1].second;
        for (int j = 1; j <= X; j++) {
            int acc_compradas = min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion));
            int beneficio_con_compra = empresas[empresa].beneficio * acc_compradas - empresas[empresa].comision * acc_compradas;
            beneficio_con_compra += PD[i-1][j - acc_compradas * empresas[empresa].precio_accion];
            PD[i][j] = max(PD[i-1][j], beneficio_con_compra);
        }
    }

    // Obtener las acciones compradas
    vector<int> acciones_compradas(N, 0);
    int j = X;
    int k = N;
    /* for (int i = N; i > 0; i--) {
        int empresa = orden[i-1].second;
        int acc_compradas = min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion));
        acciones_compradas[empresa] = acc_compradas;
        j -= acc_compradas * empresas[empresa].precio_accion;
    } */

    while (k>0) {
        int empresa = orden[k-1].second;
        if (PD[k][j] != PD[k-1][j]){
            int acc_compradas = min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion));
            /* acciones_compradas[empresa] = acc_compradas;
            j -= acc_compradas * empresas[empresa].precio_accion; */
            int dinero = j- (j - acc_compradas * empresas[empresa].precio_accion);
            acciones_compradas[empresa] = dinero / empresas[empresa].precio_accion;
            j -= acciones_compradas[empresa] * empresas[empresa].precio_accion;
        }
        k--;
    }

    // Imprimir el resultado
    cout << "Beneficio máximo: " << PD[N][X] << endl;
    for (int i = 0; i < N; i++) {
        cout << "Acciones compradas en la empresa " << i+1 << ": " << acciones_compradas[i] << endl;
    }
}

int Beneficio(const Empresa & empresa) {
    return empresa.beneficio * empresa.precio_accion;
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
        fi >> empresas[i].acciones_disponibles >> empresas[i].precio_accion
           >> empresas[i].beneficio >> empresas[i].comision;
    }

    // Poner el beneficio y la comision como porcentajes
    for (int i=0; i<N; ++i) {
        empresas[i].beneficio /= 100;
        empresas[i].comision /= 100;
    }

    fi.close();

    for (int i=0; i<N; ++i) {
        cout << "Empresa " << i+1 << ": " << empresas[i].acciones_disponibles << " acciones disponibles, "
             << empresas[i].precio_accion << " euros por acción, " << empresas[i].beneficio*100 << " % de beneficio, "
             << empresas[i].comision*100 << " % de comisión." << endl;
    }

    resolver(X, empresas);

    return 0;
}
