#include <iostream> // Permite la entrada y salida de datos.
#include <vector> // sirve como caja
#include <cstdlib> //para importar datos random
#include <ctime>// sirve para poner el tiempo
#include <algorithm> // para implementar algoritmos de cualquier tipo

using namespace std;

int filas, columnas;
bool encontrado = false;
bool despejado = false;
char personaje = 'o';  // Establecer el personaje por defecto como 'o'

enum Celda {
    PARED,
    CAMINO,
    CAMINO_SOLUCION
};

class Laberinto {
public:
    int filas;
    int columnas;
    vector<vector<Celda>> grid;

    // Inicializar el laberinto con todas las celdas como paredes
    Laberinto(int filas, int columnas) : filas(filas), columnas(columnas) { //genera un cuadrado con celdas como paredes
        grid.assign(filas, vector<Celda>(columnas, Celda::PARED));
    }

    void imprimir() {
        // Imprime primero el marco del laberinto en columna
        for (int j = 0; j < columnas + 2; j++) {
            cout << "|";
        }
        cout << endl;

        for (int i = 0; i < filas; i++) {
            // Imprime primero el marco del laberinto en fila
            cout << "|";

            for (int j = 0; j < columnas; j++) { 
                // se condiciona para que se ubique al margen superior izquierdo
                if (i == 0 && j == 0) {
                    cout << "E"; // Marcador de entrada
                }
                else if (i == filas - 1 && j == columnas - 1) { // si no condiciona para que se ubique al margen inferior derecho
                    cout << "S"; // Marcador de salida
                }
                else if (grid[i][j] == Celda::PARED) { // se condiciona los datos de filas y columnas para que pasen a ser paredes
                    cout << "|";
                }
                else if (grid[i][j] == Celda::CAMINO_SOLUCION) { //se condiciona los datos de fila y columna para encontrar una salida
                    cout << personaje;
                }
                else {
                    cout << " ";
                }
            }

            // Imprimir columna derecha del borde
            cout << "|" << endl;//recien imprime el otro borde derecho
        }

        // Imprimir fila inferior del borde
        for (int j = 0; j < columnas + 2; j++) {
            cout << "|"; //recien imprime el otro borde izquierdo para formar el cuadrado
        }
        cout << endl;
    }

    void generarCaminoAleatorio(int x, int y) {//los dos parametros son los indices de filas y columnas
        grid[x][y] = CAMINO;

        // Definir las posibles direcciones a explorar.
        vector<pair<int, int>> direcciones = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
        random_shuffle(direcciones.begin(), direcciones.end());//palabra reservada para direcciones aleatorias

        for (int i = 0; i < direcciones.size(); i++) { // se condiciona para generar un camino aleatorio dentromde los limites del muro
            int newX = x + direcciones[i].first * 2;
            int newY = y + direcciones[i].second * 2;

            if (newX >= 0 && newX < filas && newY >= 0 && newY < columnas && grid[newX][newY] == Celda::PARED) {//recoorre recursivamente el camino correcto generado anteriormente de S a E
                grid[x + direcciones[i].first][y + direcciones[i].second] = Celda::CAMINO;
                generarCaminoAleatorio(newX, newY);
            }
        }
    }

    void despejarSalida(int x, int y) {
        if (x <= filas - 2 && y <= columnas - 2) { // verifica que en frente de la celda de salida no haya paredes y se conecta con el camini generado
            despejado = true;
        }

        grid[x][y] = CAMINO;

        vector<pair<int, int>> direcciones = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }; //verifica los 4 lados
        random_shuffle(direcciones.begin(), direcciones.end());

        for (int i = 0; i < direcciones.size(); i++) {
            int newX = x + direcciones[i].first;
            int newY = y + direcciones[i].second;

            if (newX >= 0 && newX < filas && newY >= 0 && newY < columnas && grid[newX][newY] == Celda::PARED && despejado == false) {
                despejarSalida(newX, newY); //retorna false cuando no existe parfedes delante de la salida
            }
        }
    }

    void generar() {
        generarCaminoAleatorio(0, 0);// genera el camino aleatorio desde la coordenada 0,0
        despejarSalida(filas - 1, columnas - 1); // despeja la salida desde la coordenada opuesta a la de generar camino
    }

    void buscarSolucion(int x, int y) {
        if (x == filas - 1 && y == columnas - 1) { //encuentra la solucion y pasa a la variable definida camino solucion para utilizar luego
            encontrado = true;
        }

        grid[x][y] = CAMINO_SOLUCION; // la informacion se pasa a camino solucion pero el algoritmo lo que tiene de informacion es que tiene un camino que conecta la entrada con la salida
        vector<pair<int, int>> direcciones = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };//aqui se implementa el algoritmo de fuerza bruta backtracking

        for (int i = 0; i < direcciones.size(); i++) {
            int newX = x + direcciones[i].first;
            int newY = y + direcciones[i].second;

            if (newX >= 0 && newX < filas && newY >= 0 && newY < columnas && !encontrado && grid[newX][newY] == CAMINO) {
                buscarSolucion(newX, newY);
            }
        }
    }

    void reset() {
        for (int i = 0; i < filas; ++i) {//las informaciones sse transfieren entre si, camino solucion pasa a camino y camino pasa a camino solucion
            for (int j = 0; j < columnas; ++j) {
                if (grid[i][j] == CAMINO_SOLUCION) {
                    grid[i][j] = CAMINO;
                }
            }
        }
    }
};

int main() {
    // Inicializar la semilla del generador de números aleatorios
    srand(time(0));

    cout << "ingrese un numero de filas? : "; cin >> filas;
    cout << "ingrese un numero de columnas : "; cin >> columnas;

    Laberinto laberinto(filas, columnas);
    laberinto.generar();
    cout << endl << "Laberinto Generado: " << endl << endl;
    laberinto.imprimir();

    laberinto.buscarSolucion(0, 0);

    laberinto.reset();  // Resetear el estado del laberinto
    encontrado = false;  // Resetear el estado de búsqueda de la solución
    laberinto.buscarSolucion(0, 0);  // Buscar solución nuevamente para mostrar el camino correcto

    cout << endl << endl << "Solucion: " << endl << endl;
    laberinto.imprimir();
    cout << endl;

    return 0;
}
