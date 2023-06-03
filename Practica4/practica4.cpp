#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

// Estructura para representar una empresa
struct Empresa {
    int acciones_disponibles;
    double precio_accion;
    double beneficio;
    double comision;
};

double calculaCoste(const vector<Empresa>& empresas, const vector<int>& sol) {
    double sum = 0.0;
    for (int i=0; i<empresas.size(); i++) {
        sum += empresas[i].precio_accion * sol[i] + empresas[i].comision;
    }

    return sum;
}

double calculaBeneficio(const vector<Empresa>& empresas, const vector<int>& sol) {
    double sum = 0.0;
    for (int i=0; i<empresas.size(); i++) {
        sum += empresas[i].precio_accion * empresas[i].beneficio;
    }
    
    return sum;
}

// Función parar resolver el problema utilizando fuerza bruta
void resolverFuerzaBruta(int X, const vector<Empresa>& empresas, int index, vector<int>& sol, vector<int>& combination, double& maxBeneficio) {
    if (index == empresas.size()-1) {
        double sum = calculaBeneficio(empresas, sol);
        if (sum < X && sum > maxBeneficio) {
            maxBeneficio = sum;
            sol = combination;
        }
    }
    else {
        // Recorrer todas las combinaciones posibles
        for (int i=0; i<empresas[index].acciones_disponibles; i++) {
            // Insertar posible solución
            sol.push_back(i);

            resolverFuerzaBruta(X, empresas, index+1, sol, combination, maxBeneficio);

            // Eliminar el último elemento del vector para seguir probando combinaciones
            combination.pop_back();
        }
    }
}

// Función encargada de pasar los argumentos necesarios para la primera ejecución
// del algoritmo de fuerza bruta
vector<int> resolverFB(int& X, vector<Empresa>& empresas) {
    vector<int> sol, combination;
    double maxBeneficio = INT_MIN;

    resolverFuerzaBruta(X, empresas, 0, sol, combination, maxBeneficio);

    return sol;
}

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
    vector<vector<double>> PD(N , vector<double>(X + 1, 0));

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
            double beneficio=0;
            if (j >= empresas[empresa].precio_accion + empresas[empresa].comision){

                if (j == empresas[empresa].precio_accion + empresas[empresa].comision){
                    beneficio = empresas[empresa].beneficio * empresas[empresa].precio_accion;
                }
                else if (PD[i][j-empresas[empresa].precio_accion - empresas[empresa].comision] ==
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

    cout << PD[N-1][X] << endl;
    cout << PD[N-1][X - 19*empresas[4].precio_accion-19*empresas[4].comision] << endl;
    cout << PD[N-1][X - 20*empresas[4].precio_accion-20*empresas[4].comision] << endl;
    cout << PD[N-2][X - 20*empresas[4].precio_accion-20*empresas[4].comision] << endl;

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

// Función auxiliar para resolver el problema utilizando programación dinámica
void PDAuxiliar(int X, const vector<Empresa>& empresas) {
    int N = empresas.size();

    // Ordenar las empresas por la razón beneficio / precio en orden descendente
    vector<pair<double, int>> orden;
    for (int i = 0; i < N; i++) {
        double razon = static_cast<double>(empresas[i].beneficio) / empresas[i].precio_accion;
        //double razon = empresas[i].precio_accion;
        //double razon = empresas[i].beneficio;
        orden.push_back({razon, i});
    }
    //sort(orden.rbegin(), orden.rend());
    sort(orden.begin(), orden.end());

    // Imprimir el orden
    cout << endl << "Orden de las empresas: " << endl;
    for (int i = 0; i < N; i++) {
        cout << "Empresa " << orden[i].second+1 << ": " << orden[i].first << endl;
    }

    cout << endl;

    // Inicializar la matriz de programación dinámica
    vector<vector<double>> PD(N , vector<double>(X + 1, 0));
    vector<vector<int>> accionesCompradas(N , vector<int>(X + 1, 0));

    // Rellenar casos base
    for (int i = 0; i < N; i++) {
        PD[i][0] = 0;
    }
    for (int j = 0; j <= X; j++) {
        int empresa = orden[0].second;
        PD[0][j] = 0;
        for (int k=1; k<=empresas[empresa].acciones_disponibles; k++){

            double coste = k*(empresas[empresa].precio_accion + empresas[empresa].comision);

            if (j >= coste){
                double beneficio = k*empresas[empresa].precio_accion*empresas[empresa].beneficio;

                if (PD[0][j] < beneficio){
                    PD[0][j] = beneficio;
                    accionesCompradas[empresa][j] = k;
                }
            }
        }
    }

    // Calcular el beneficio máximo
    for (int i=1 ; i<N; i++){
        int empresa = orden[i].second;
        for (int j=1; j<=X; j++){

            for (int k=1; k<=empresas[empresa].acciones_disponibles; k++){

                double coste = k*(empresas[empresa].precio_accion + empresas[empresa].comision);
                double beneficio = 0;

                if (j >= coste){
                    beneficio = k*empresas[empresa].precio_accion*empresas[empresa].beneficio;
                    beneficio += PD[i-1][j-coste];
                }

                if (k==1){
                    PD[i][j] = max(PD[i-1][j], beneficio);
                } else{
                    PD[i][j] = max(PD[i][j], beneficio);
                }

                if (PD[i][j] == beneficio && beneficio != 0)
                    accionesCompradas[empresa][j] = k;
            }
        }
    }

    // Obtener las acciones compradas
    vector<int> acciones_compradas(N, 0);
    int dinero = X;
    for (int i=N-1; i>=0; i--){
        int empresa = orden[i].second;
        acciones_compradas[empresa] = accionesCompradas[empresa][dinero];
        dinero -= acciones_compradas[empresa]*empresas[empresa].precio_accion + acciones_compradas[empresa]*empresas[empresa].comision;
    }

    // Imprimir el resultado
    cout << endl << "Beneficio máximo: " << PD[N-1][X] << endl;
    for (int i = 0; i < N; i++) {
        cout << "Acciones compradas en la empresa " << i+1 << ": " << acciones_compradas[i] << endl;
    }

}

// Función principal
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
    PDAuxiliar(X, empresas);

    return 0;
}
