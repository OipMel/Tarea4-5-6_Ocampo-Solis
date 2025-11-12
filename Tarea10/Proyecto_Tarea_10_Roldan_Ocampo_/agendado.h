#ifndef AGENDADO_H
#define AGENDADO_H

#include <iostream>
#include <string>
using namespace std;

struct agendado {
    string alias;
    string cbu;
    string nombre;
    string cuit;
    string banco;
    bool encontrado;
};

#endif // AGENDADO_H