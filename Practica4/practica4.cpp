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
    cout << "N = " << N << endl;
    vector<float> acciones_compradas(N, 0);

    // Ordenar las empresas por la razón beneficio / precio en orden descendente
    vector<pair<double, int>> orden;
    for (int i = 0; i < N; i++) {
        //double razon = static_cast<double>(empresas[i].beneficio) / empresas[i].precio_accion;
        double razon = empresas[i].precio_accion;
        orden.push_back({razon, i});
    }
    sort(orden.rbegin(), orden.rend());

    // Imprimir el orden
    for (int i = 0; i < N; i++) {
        cout << "Empresa " << orden[i].second+1 << ": " << orden[i].first << endl;
    }

    // Inicializar la matriz de programación dinámica
    vector<vector<int>> PD(N , vector<int>(X + 1, 0));

    // Rellenar casos base
    for (int i = 0; i < N; i++) {
        PD[i][0] = 0;
    }
    for (int j = 0; j <= X; j++) {
        int empresa = orden[0].second;
        PD[0][j] = empresas[empresa].beneficio *
                   min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion)) *
                   empresas[empresa].precio_accion -
                   empresas[empresa].comision * min(empresas[empresa].acciones_disponibles,
                   static_cast<int>(j / empresas[empresa].precio_accion));
        //cout << "PD[0][" << j << "] = " << PD[0][j] << endl;
    }

    // Calcular el beneficio máximo
    for (int i = 1; i < N; i++) {
        int empresa = orden[i-1].second;
        for (int j = 1; j <= X; j++) {
            int acc_compradas = min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion));
            int beneficio_con_compra = empresas[empresa].beneficio * acc_compradas * empresas[empresa].precio_accion
                                     - empresas[empresa].comision * acc_compradas;
            beneficio_con_compra += PD[i-1][j - acc_compradas * empresas[empresa].precio_accion];
            PD[i][j] = max(PD[i-1][j], beneficio_con_compra);
        }
    }

    // Obtener las acciones compradas
    int j = X;
    int k = N-1;
    /* for (int i = N; i > 0; i--) {
        int empresa = orden[i-1].second;
        int acc_compradas = min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion));
        acciones_compradas[empresa] = acc_compradas;
        j -= acc_compradas * empresas[empresa].precio_accion;
    } */

    while (k>0) {
        int empresa = orden[k].second;
        if (PD[k][j] != PD[k-1][j]){
            int acc_compradas = min(empresas[empresa].acciones_disponibles, static_cast<int>(j / empresas[empresa].precio_accion));
            cout << "Acciones compradas en la empresa " << empresa+1 << ": " << acc_compradas << endl;
            /* acciones_compradas[empresa] = acc_compradas;
            j -= acc_compradas * empresas[empresa].precio_accion; */
            //int dinero = j- (j - acc_compradas * empresas[empresa].precio_accion);
            //acciones_compradas[empresa] = dinero / empresas[empresa].precio_accion;
            acciones_compradas[empresa] = acc_compradas;
            j -= acciones_compradas[empresa] * empresas[empresa].precio_accion;
        }
        k--;
    }

    /* acciones_compradas[orden[0].second] = min(empresas[orden[0].second].acciones_disponibles,
                                          static_cast<int>(j / empresas[orden[0].second].precio_accion)); */

    acciones_compradas[orden[0].second] = min(empresas[orden[0].second].acciones_disponibles,
                                        static_cast<int>(j / empresas[orden[0].second].precio_accion));

    // Imprimir el resultado
    cout << "Beneficio máximo: " << PD[4][X] << " en empresa: " << orden[4].second+1 << endl;
    cout << "Beneficio anterior: " << PD[3][X] << " en empresa: " << orden[3].second+1 << endl;
    cout << "Beneficio anterior: " << PD[2][X] << " en empresa: " << orden[2].second+1 << endl;
    cout << "Beneficio anterior: " << PD[1][X] << " en empresa: " << orden[1].second+1 << endl;
    cout << "Beneficio anterior: " << PD[0][X] << " en empresa: " << orden[0].second+1 << endl;
    cout << endl;
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
