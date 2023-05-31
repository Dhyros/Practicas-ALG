#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Estructura para representar una empresa
struct Empresa {
    int acciones_disponibles;
    double precio_accion;
    double beneficio;
    double comision;
};

// Función para resolver el problema utilizando programación dinámica
void resolver(int X, const vector<Empresa>& empresas) {
    int N = empresas.size();

    // Ordenar las empresas por la razón beneficio / precio en orden descendente
    vector<pair<double, int>> orden;
    for (int i = 0; i < N; i++) {
        double razon = static_cast<double>(empresas[i].beneficio) / empresas[i].precio_accion;
        //double razon = empresas[i].precio_accion;
        //double razon = empresas[i].beneficio;
        orden.push_back({razon, i});
    }
    sort(orden.rbegin(), orden.rend());
    //sort(orden.begin(), orden.end());

    // Imprimir el orden
    cout << endl << "Orden de las empresas: " << endl;
    for (int i = 0; i < N; i++) {
        cout << "Empresa " << orden[i].second+1 << ": " << orden[i].first << endl;
    }

    cout << endl;

    // Inicializar la matriz de programación dinámica
    vector<vector<int>> PD(N , vector<int>(X + 1, 0));

    // Rellenar casos base
    for (int i = 0; i < N; i++) {
        PD[i][0] = 0;
    }
    for (int j = 0; j <= X; j++) {
        int empresa = orden[0].second;
        int n = j/static_cast<int>(empresas[empresa].precio_accion + empresas[empresa].comision);
        if (n > empresas[empresa].acciones_disponibles){
            PD[0][j] = empresas[empresa].acciones_disponibles*empresas[empresa].beneficio * empresas[empresa].precio_accion;
        }
        else {
            PD[0][j] = n*empresas[empresa].beneficio * empresas[empresa].precio_accion;
        }
    }

    // Calcular el beneficio máximo
    for (int i = 1; i < N; i++) {
        int empresa = orden[i].second;
        for (int j = 1; j <= X; j++) {
            int beneficio=0;
            if (j >= empresas[empresa].precio_accion + empresas[empresa].comision){

                if (PD[i][j-empresas[empresa].precio_accion - empresas[empresa].comision] ==
                    PD[i-1][j-empresas[empresa].precio_accion - empresas[empresa].comision]){

                    beneficio = empresas[empresa].beneficio * empresas[empresa].precio_accion;
                    beneficio += PD[i][j - empresas[empresa].precio_accion - empresas[empresa].comision];
                }
                else{
                    int acc = 0;
                    while ((PD[i][j - (acc+1)*(empresas[empresa].precio_accion + empresas[empresa].comision)] !=
                           PD[i-1][j - (acc+1)*(empresas[empresa].precio_accion + empresas[empresa].comision)])){
                        acc++;
                    }
                    if (acc < empresas[empresa].acciones_disponibles){
                        beneficio = empresas[empresa].beneficio * empresas[empresa].precio_accion;
                        beneficio += PD[i][j - empresas[empresa].precio_accion - empresas[empresa].comision];
                    }
                    else {
                        beneficio = PD[i][j - empresas[empresa].precio_accion - empresas[empresa].comision];
                    }
                }
            }

            PD[i][j] = max(PD[i-1][j], beneficio);

        }
    }

    // Obtener las acciones compradas
    int j = X;
    int k = N-1;
    vector<int> acciones_compradas(N, 0);

    while (k>=0) {
        int empresa = orden[k].second;
        if (k > 0 && PD[k][j] != PD[k-1][j] &&
                            PD[k][j] != PD[k][j-empresas[empresa].precio_accion - empresas[empresa].comision]){
            acciones_compradas[empresa]++;
            j = (j - empresas[empresa].precio_accion - empresas[empresa].comision);
        }
        else if (k > 0 && PD[k][j] != PD[k-1][j]){
            j = (j - empresas[empresa].precio_accion - empresas[empresa].comision);
        }
        else if (k == 0 && PD[k][j] != PD[k][j-empresas[empresa].precio_accion - empresas[empresa].comision] ) {
            acciones_compradas[empresa]++;
            j = (j - empresas[empresa].precio_accion - empresas[empresa].comision);
        }
        else
            k--;
    }

    // Imprimir el resultado
    cout << endl << "Beneficio máximo: " << PD[N-1][X] << endl;
    for (int i = 0; i < N; i++) {
        cout << "Acciones compradas en la empresa " << i+1 << ": " << acciones_compradas[i] << endl;
    }
}

// Función recursiva para calcular el beneficio máximo
double calcularBeneficioMaximo(int X, vector<Empresa>& empresas, vector<int>& accionesCompradas, int i) {
    // Caso base: si ya hemos recorrido todas las empresas o no tenemos dinero para invertir
    if (i >= empresas.size() || X <= 0) {
        return 0;
    }

    // Caso 1: No comprar acciones de la empresa actual
    double beneficioNoComprar = calcularBeneficioMaximo(X, empresas, accionesCompradas, i + 1);

    // Caso 2: Comprar acciones de la empresa actual si hay suficientes fondos
    double beneficioComprar = 0;
    if (X >= (accionesCompradas[i] * empresas[i].precio_accion /* + empresas[i].comision */)) {
        accionesCompradas[i]++;
        beneficioComprar = (accionesCompradas[i] * empresas[i].precio_accion * empresas[i].beneficio) -
        empresas[i].comision*accionesCompradas[i]*empresas[i].precio_accion + calcularBeneficioMaximo(X -
        (accionesCompradas[i] * empresas[i].precio_accion), empresas, accionesCompradas, i);
        accionesCompradas[i]--;
    }

    // Devolver el máximo beneficio entre los dos casos
    return max(beneficioNoComprar, beneficioComprar);
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

    // Poner el beneficio como porcentajes
    for (int i=0; i<N; ++i)
        empresas[i].beneficio /= 100;


    fi.close();

    for (int i=0; i<N; ++i) {
        cout << "Empresa " << i+1 << ": " << empresas[i].acciones_disponibles << " acciones disponibles, "
             << empresas[i].precio_accion << " euros por acción, " << empresas[i].beneficio*100 << " % de beneficio, "
             << empresas[i].comision << " euros de comisión por cada acción." << endl;
    }

    resolver(X, empresas);
    vector<int> accionesCompradas(N, 0);
    cout << endl << "Beneficio máximo: " << calcularBeneficioMaximo(X, empresas, accionesCompradas, 0) << endl;

    // Imprimir las acciones compradas para cada empresa
    cout << "Acciones compradas:" << endl;
    for (int i = 0; i < N; i++) {
        if (accionesCompradas[i] > 0) {
            cout << "Empresa " << i + 1 << ": " << accionesCompradas[i] << " acciones" << endl;
        }
    }

    return 0;
}
