#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>
using namespace std;

// Estructura de datos personales de un cliente
struct Cliente {
    string nombre;
    string direccion;
    string telefono;
    string correo;
    string clave;   // contraseña cifrada o PIN
    double saldo;
};

#endif // CLIENTE_H
