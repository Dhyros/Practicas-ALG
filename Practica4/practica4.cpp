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

double calculaCoste(const vector<Empresa>& empresas, const vector<int>& combination) { //O(e) e = empresas.size()
    double sum = 0.0;
    for (int i=0; i<empresas.size(); i++) {
        sum += (empresas[i].precio_accion + empresas[i].comision) * combination[i];
    }

    return sum;
}

double calculaBeneficio(const vector<Empresa>& empresas, const vector<int>& combination) { //O(e) e = empresas.size()
    double sum = 0.0;
    for (int i=0; i<empresas.size(); i++) {
        sum += empresas[i].precio_accion * empresas[i].beneficio * combination[i];
    }

    return sum;
}

// Función parar resolver el problema utilizando fuerza bruta
void resolverFuerzaBruta(int X, const vector<Empresa>& empresas, int index, vector<int>& sol, vector<int>& combination,
                         double& maxBeneficio) {  // O(a^e)

    if (index == empresas.size()) { // O(e) e = empresas.size()
        double sum = calculaBeneficio(empresas, combination);
        if (calculaCoste(empresas, combination) <= X && sum > maxBeneficio) {
            maxBeneficio = sum;
            sol = combination;
        }
    }
    else {
        // Recorrer todas las combinaciones posibles
        for (int i=0; i<=empresas[index].acciones_disponibles; i++) { // T(e) = a * (T(e-1) + 1) -----> O(a^e)
            // Insertar posible solución
            combination.push_back(i); // O(1)

            resolverFuerzaBruta(X, empresas, index+1, sol, combination, maxBeneficio); //T(e-1)

            // Eliminar el último elemento del vector para seguir probando combinaciones
            combination.pop_back(); // O(1)
        }
    }

}

// Función encargada de pasar los argumentos necesarios para la primera ejecución
// del algoritmo de fuerza bruta
vector<int> resolverFB(int& X, vector<Empresa>& empresas) { // O(a^e)
    vector<int> sol, combination; // O(1)
    double maxBeneficio = INT_MIN; // O(1)

    resolverFuerzaBruta(X, empresas, 0, sol, combination, maxBeneficio); // O(a^e)

    return sol;
}

// Función auxiliar para resolver el problema utilizando programación dinámica
void PDAuxiliar(int X, const vector<Empresa>& empresas) {  // O(e * X * a)
    int N = empresas.size();

    // Ordenar las empresas por la razón beneficio / precio en orden descendente
    vector<pair<double, int>> orden; // O(1)
    for (int i = 0; i < N; i++) { // O(e) ----> e = empresas.size()

        double razon = static_cast<double>(empresas[i].beneficio) / empresas[i].precio_accion; // O(1)
        orden.push_back({razon, i}); // O(1)
    
    }

    sort(orden.begin(), orden.end()); // O(e*log(e))

    // Imprimir el orden
    cout << endl << "Orden de las empresas: " << endl; // O(1)
    for (int i = 0; i < N; i++) { // O(e)
        cout << "Empresa " << orden[i].second+1 << ": " << orden[i].first << endl; // O(1)
    }
    cout << endl; // O(1)

    // Inicializar la matriz de programación dinámica
    vector<vector<double>> PD(N , vector<double>(X + 1, 0));

    // Rellenar casos base
    for (int i = 0; i < N; i++) { // O(e) e = empresas.size()
        PD[i][0] = 0;
    }

    for (int j = 0; j <= X; j++) { // O(X * a)
        int empresa = orden[0].second;
        PD[0][j] = 0;
        int num_acciones = j / (empresas[empresa].precio_accion + empresas[empresa].comision);
        if (num_acciones <= empresas[empresa].acciones_disponibles)
            PD[0][j] = num_acciones * empresas[empresa].precio_accion * empresas[empresa].beneficio;
        else
            PD[0][j] = empresas[empresa].acciones_disponibles * empresas[empresa].precio_accion * empresas[empresa].beneficio;
    }

    // Calcular el beneficio máximo
    for (int i=1 ; i<N; i++){ // O(e * X * a)
        int empresa = orden[i].second;
        for (int j=1; j<=X; j++){ // O(X * a)

            for (int k=1; k<=empresas[empresa].acciones_disponibles; k++){ // O(a) -----> a = numero de acciones de la empresa

                double coste = k*(empresas[empresa].precio_accion + empresas[empresa].comision); // O(1)
                double beneficio = 0; // O(1)

                if (j >= coste){ // O(1)
                    beneficio = k*empresas[empresa].precio_accion*empresas[empresa].beneficio; // O(1)
                    beneficio += PD[i-1][j-coste]; // O(1)
                }

                if (k==1) // O(1)
                    PD[i][j] = max(PD[i-1][j], beneficio); // O(1)
                else
                    PD[i][j] = max(PD[i][j], beneficio); // O(1)

            }
        }
    }

    // Obtener las acciones compradas
    vector<int> acciones_compradas(N, 0);
    int dinero = X;
    int i = N-1;
    while (i>=0){
        int empresa = orden[i].second;

        if ( i>0 && PD[i][dinero] != PD[i-1][dinero] ){

            bool parar = false;

            for (int k = 1; k <= empresas[empresa].acciones_disponibles && !parar; k++){ // O(a)
                if (PD[i][dinero] == k*empresas[empresa].precio_accion*empresas[empresa].beneficio +
                        PD[i-1][dinero-k*(empresas[empresa].precio_accion + empresas[empresa].comision)]){ // O(1)
                    acciones_compradas[empresa] = k;
                    dinero -= k*(empresas[empresa].precio_accion + empresas[empresa].comision);
                    i--;
                    parar = true;
                }
           }

        }
        else if (i==0 && PD[i][dinero] != 0){

            bool parar = false;

            for (int k = 1; k <= empresas[empresa].acciones_disponibles && !parar; k++){ // O(a)
                if (PD[i][dinero] == k*empresas[empresa].precio_accion*empresas[empresa].beneficio){ // O(1)
                    acciones_compradas[empresa] = k;
                    dinero -= k*(empresas[empresa].precio_accion + empresas[empresa].comision);
                    parar = true;
                }
            }
        }
        else
            i--;
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
             << empresas[i].comision << " euro(s) de comisión por cada acción." << endl;
    }

    // resolver(X, empresas);
    PDAuxiliar(X, empresas);

    vector<int> v = resolverFB(X, empresas);

    cout << endl << "-----------------------------------------" << endl;
    cout << "FUERZA BRUTA" << endl;
    cout << "Beneficio máximo: " << calculaBeneficio(empresas, v) << endl;
    cout << "Acciones a comprar:" << endl;
    for (int i=0; i<empresas.size(); i++) {
        cout << "    Empresa " << i+1 << ": " << v[i] << endl;
    }


    return 0;
}
