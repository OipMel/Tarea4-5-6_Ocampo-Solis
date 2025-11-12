#ifndef kmp_h
#define kmp_h
#include <iostream>
#include <map>
#include <vector>
#include<string>
#include<fstream>
#include "agendado.h"
//para comprimir LZW para comprimir transferencia(listo)
//Busqueda KMP para movimientos
//Fuerza bruta busqueda de contactos
//Cifrado playfair 
using namespace std;

struct movimiento {
    string fecha;
    float cantidad;
    agendado persona;
};
vector<int> LZW_comprimir(string texto) {
    map<string, int> dic;
    for (int i = 0; i < 256; i++) {
        dic[string(1, i)] = i;
    }
    string p = "";
    vector<int> salida;
    int codigo = 256;
    for (char c : texto) {
        string pc = p + c;
        if (dic.count(pc)) {
            p = pc;
        }
        else {
            salida.push_back(dic[p]);
            dic[pc] = codigo++;
            p = string(1, c);
        }
    }
    if (!p.empty()) {
        salida.push_back(dic[p]);
    }
    return salida;
}
void comprimir(agendado datos, string monto, string fecha) {
    ofstream comprobante("Comprobante.txt");
    if (comprobante.is_open()) {
        string texto =
            "Alias: " + datos.alias + "\n" +
            "CBU: " + datos.cbu + "\n" +
            "Nombre: " + datos.nombre + "\n" +
            "CUIT: " + datos.cuit + "\n" +
            "Banco: " + datos.banco + "\n";
        vector<int>comp = LZW_comprimir(texto);
        for (int x : comp) {
            comprobante << x;
        }
    }
    else {
        cout << "No existe el archivo" << endl;
    }
}
agendado BuscarAgendado(string alias) {
    ifstream buscar("alias.txt");
    agendado datos = { "","","","","",false };
    if (!buscar.is_open()) {
        cout << "Error al abrir el archivo" << endl;
    }
    else {
        string linea;
        string encontrado = alias;
        while (getline(buscar, linea)) {
            if (linea.find("Alias:") != string::npos) {
                datos.alias = linea.substr(6);
                if (datos.alias == encontrado) {
                    datos.alias = encontrado;
                    getline(buscar, linea); datos.cbu = linea.substr(0);
                    getline(buscar, linea); datos.nombre = linea.substr(0);
                    getline(buscar, linea); datos.cuit = linea.substr(0);
                    getline(buscar, linea); datos.banco = linea.substr(0);
                    datos.encontrado = true;
                    break;
                }
            }
            else {
                cout << "El alias " << encontrado << " no fue encontrado en la agenda lamentable, agendalo loco" << endl;
                buscar.close();
            }
        }
    }
    return datos;
}

void transferencia(agendado datos) {
    ofstream historial("Historial.txt", ios::app);
    if (historial.is_open()) {
        string fecha;
        cout << "Ingrese la fecha de hoy " << endl;
        getline(cin, fecha);
        string monto;
        string respuesta;
        if (datos.encontrado == true) {
            cout << "-----------------Transferencia---------------        " << endl;
            cout << "ingrese el monto" << endl;
            getline(cin, monto);
            historial << "Fecha: " << fecha << endl;//automaticametne se ponga la fecha actual
            historial << "Alias: " << datos.alias << endl;//aca se pone el alias ya buscado 
            historial << datos.cbu << endl;
            historial << datos.nombre << endl;//aca se pone nombre ya buscado
            historial << datos.cuit << endl;//aca se pone el cuit ya buscado 
            historial << datos.banco << endl;//aca se pone el banco ya buscado 
            historial << "Monto: " << monto << endl;
            historial << "------------------------------------" << endl;
            historial.close();
            cout << "Tranferencia enviada! super cute" << endl;
            cout << "¿Desea generar un comprobante?" << endl;
            cin >> respuesta;
            if (respuesta == "si") {
                comprimir(datos, monto, fecha);
            }
            else {
                cout << "Sos re sorete amigo, me re esmere en el LZW" << endl;
            }
        }
    }
    else {
        cout << "No se pudo abrir el archivo xdxd" << endl;
    }
}
vector<int> calcularfallo(const string& p) {
    int m = p.size();
    vector<int> fallo(m);
    int j = 0;
    for (int i = 1; i < m; i++) {
        while (j > 0 && p[i] != p[j]) {
            j = fallo[j - 1];
        }
        if (p[i] == p[j]) {
            j++;
        }
        fallo[i] = j;
    }
    return fallo;
}
//para saber si existe fecha coincidente 
bool KMP(const string& texto, const string& patron) {
    vector<int> fallo = calcularfallo(patron);
    int j = 0;
    for (int i = 0; i < texto.size(); i++) {
        while (j > 0 && texto[i] != patron[j]) {
            j = fallo[j - 1];
        }
        if (texto[i] == patron[j]) {
            j++;
        }
        if (j == patron.size()) {
            return true;
        }
    }
    return false;
}
void busqueda_mov_KMP() {
    string fechaBuscada;
    cout << "Ingrese la fecha que desea buscar (formato exacto): ";
    cin.ignore();
    getline(cin, fechaBuscada);
    ifstream archivo("Historial.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de historial." << endl;
        return;
    }
    string linea;
    bool mostrar = false;
    vector<string> movimiento;

    while (getline(archivo, linea)) {
        if (linea.find("Fecha: ") == 0) {
            // Si hay un movimiento anterior que coincide, lo mostramos
            if (!movimiento.empty() && mostrar) {
                for (const string& l : movimiento) {
                    cout << l << endl;
                }
                cout << "------------------------------------" << endl;
            }
            // Reiniciamos para el nuevo movimiento
            movimiento.clear();
            mostrar = false;
            string fechaHistorial = linea.substr(7); // Quita "Fecha: "
            movimiento.push_back(linea);

            if (KMP(fechaHistorial, fechaBuscada)) {
                mostrar = true;
            }
        }
        else {
            movimiento.push_back(linea);
        }
    }
    if (!movimiento.empty() && mostrar) {
        for (const string& l : movimiento) {
            cout << l << endl;
        }
        cout << "------------------------------------" << endl;
    }
    archivo.close();
}

#endif