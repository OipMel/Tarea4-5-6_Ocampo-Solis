#include "Grafo_Combinado.h"
#include "Prim.h"
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <limits>
using namespace std;

bool letras(const string& texto) { // solo acepta letras
    for (char c : texto) {
        if (!isalpha(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}

int main() {
    int opc;
    Grafo grafo;
    bool grafo_cargado = false;

    grafo.cargarArchivo();

    do {
        cout << "\n\t\t\t -Menu de Armado de Tendidos de Telecomunicaciones-\n";
        cout << "\t\t-----------------------------------------------------------------\n";
        cout << "\t\t1. Cargar mapa de tendido\n";
        cout << "\t\t2. Resolver mapa y crear su monticulo binario \n";
        cout << "\t\t0. Salir\n";
        cout << "\t\t-----------------------------------------------------------------\n";
        cout << "\t\tElige una opcion: ";
        cin >> opc;

        switch (opc) {
        case 1: {

            int sub;
            system("cls");
            do {
                //muestra el grafo e indica si es ponderado y direccional
                cout << "\t\t-----------------------------------------------------------------\n";
                grafo.mostrarArchivo();
                if (grafo.cargarArchivo() == 0) {
                    cout << "\t\t-----------------------------------------------------------------\n";
                    cout << "\t\t\t\tPonderado: " << (grafo.es_ponderado() ? "Si" : "No")
                        << " / Direccional: " << (grafo.es_direccional() ? "Si" : "No") << "\n";
                    grafo_cargado = true;
                }
                else {
                    cout << "\t\tNo hay datos en el archivo\n";
                }

                cout << "\t\t-----------------------------------------------------------------\n";
                cout << "\t\t                            Menu de mapa                      \n";
                cout << "\t\t-----------------------------------------------------------------\n";
                cout << "\t\t1. Hacer uno nuevo (borrar todo)\n";
                cout << "\t\t2. Agregar nodo (usuario)\n";
                cout << "\t\t3. Agregar cableado (conexion)\n";
                cout << "\t\t0. Volver al menu principal\n";
                cout << "\t\tElige: ";
                cin >> sub;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (sub == 1) {//empezar uno nuevo
                    system("cls");
                    grafo.borrar_archivo();
                    grafo.vaciar();
                    grafo = Grafo();
                    grafo_cargado = false;
                    cout << "\t\t-----------------------------------------------------------------\n";
                    cout << "\t\t                Se borraron nodos y cableados\n";
                    cout << "\t\t-----------------------------------------------------------------\n";

                    char es_dir = 's';
                    grafo.set_direccional(es_dir == 's' || es_dir == 'S');

                    // cargar nodos
                    int cant_nodos = 0;
                    cout << "\t\t-Cuantos nodos (usuarios) desea agregar?: ";
                    cin >> cant_nodos;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    for (int i = 0; i < cant_nodos; ++i) {
                        string nodo;
                        do {
                            cout << "\t\t-Nodo a cargar: ";
                            cin >> nodo;
                            if (!letras(nodo)) {
                                cout << "\t\t*Error: solo letras A-Z/a-z\n";
                            }
                        } while (!letras(nodo));

                        if (!grafo.duplicado(nodo)) {
                            grafo.agregar_nodo(nodo);
                            grafo_cargado = true;
                        }
                        else {
                            cout << "\t\tEl nodo ya existe, ponga otro\n";
                            --i;
                        }
                    }

                    // Cargar aristas 
                    cout << "\t\t-----------------------------------------------------------------\n";
                    cout << "\n\t\t-Cargue conexiones (escriba '*' para salir)\n";
                    while (true) {
                        string a, b, peso_texto;
                        int peso = 0;

                        cout << "\t\t-Usuario 1: ";
                        cin >> a;
                        if (a == "*") break;

                        cout << "\t\t-Usuario 2: ";
                        cin >> b;
                        if (!letras(a) || !letras(b)) {
                            cout << "\t\t*Error, caracter invalido\n";
                            continue;
                        }

                        cout << "\t\t-Coste de conexion (enter para ingresar sin coste): ";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        getline(cin, peso_texto);

                        if (!peso_texto.empty()) {//si pone enter o se equivoca, se coloca 0
                            try { peso = stoi(peso_texto); }
                            catch (...) {
                                cout << "\t\t*Coste invalido, se guardara sin coste\n";
                                peso = 0;
                            }
                        }

                        bool bidireccional = !grafo.es_direccional();
                        if (grafo.es_direccional()) {
                            int dir_opc = 3;
                            bidireccional = true;//ambas direcciones
                        }

                        if (grafo.duplicado(a) && grafo.duplicado(b)) {
                            grafo.agregar_arista(a, b, peso, bidireccional);
                            grafo_cargado = true;
                        }
                        else {
                            cout << "\t\t*Error, ambos usuarios deben existir\n";
                        }
                    }

                    // Guardar
                    if (!grafo.guardar("grafo.txt")) {
                        cout << "\t\t*Error al guardar grafo.txt\n";
                    }
                    else {
                        cout << "\t\t-----------------------------------------------------------------\n";
                        cout << "\t\tmapa guardado en grafo.txt\n";
                        cout << "\t\t-----------------------------------------------------------------\n";
                        system("cls");
                        grafo.cargarArchivo();
                    }

                }
                else if (sub == 2) { //solo agrega nodo
                    system("cls");
                    string nodo;
                    do {
                        cout << "\t\tIngrese el usuario a conectar: ";
                        cin >> nodo;
                        if (!letras(nodo)) cout << "\t\t*Error: solo A-Z a-z\n";
                    } while (!letras(nodo));

                    if (!grafo.duplicado(nodo)) {
                        grafo.agregar_nodo(nodo);
                        grafo_cargado = true;
                        cout << "\t\t*Usuario agregado\n";
                    }
                    else {
                        cout << "\t\t*El usuario ya existe.\n";
                    }

                    cout << "\n\t\tGuardar cambios? (s/n): ";
                    char res;
                    cin >> res;
                    if (res == 's' || res == 'S') {
                        if (!grafo.guardar("grafo.txt"))
                            cout << "\t\t*Error al guardar\n";
                        else {
                            system("cls");
                            grafo.cargarArchivo();
                        }
                    }

                }
                else if (sub == 3) {// solo arista
                    system("cls");
                    string a, b, peso_texto;
                    int peso = 0;

                    cout << "\t\t-Ingrese usuario 1: ";
                    cin >> a;
                    cout << "\t\t-Ingrese usuario 2: ";
                    cin >> b;

                    if (!letras(a) || !letras(b)) {
                        cout << "\t\t*Error caracter invalido\n";
                        continue;
                    }

                    cout << "\t\t-Ingrese coste (enter para ingresar sin coste): ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, peso_texto);

                    if (!peso_texto.empty()) {
                        try { peso = stoi(peso_texto); }
                        catch (...) {
                            cout << "\t\Coste invalido, se toma como sin coste\n";
                            peso = 0;
                        }
                    }

                    bool bidireccional = !grafo.es_direccional();
                    if (grafo.es_direccional()) {
                        bidireccional = true;
                    }

                    if (grafo.duplicado(a) && grafo.duplicado(b)) {
                        grafo.agregar_arista(a, b, peso, bidireccional);
                        grafo_cargado = true;
                    }
                    else {
                        cout << "\t\t*Ambos usuarios deben existir\n";
                    }

                    cout << "\n\t\t*Guardar cambios? (s/n): ";
                    char r;
                    cin >> r;
                    if (r == 's' || r == 'S') {
                        if (!grafo.guardar("grafo.txt"))
                            cout << "\t\t*Error al guardar\n";
                        else {
                            system("cls");
                            grafo.cargarArchivo();
                        }
                    }
                }

            } while (sub != 0);
            break;
        }

        case 2: {
            system("cls");
            ejecutar_prim(grafo);
            break;
        }
        case 0:
            cout << "Saliendo del programa...\n";
            break;

        default:
            cout << "Opcion invalida\n";
        }

        if (opc != 0) {
            cin.ignore();
            cout << "\nPresiona enter para continuar...";
            cin.get();
            system("cls");
        }

    } while (opc != 0);

    return 0;
}