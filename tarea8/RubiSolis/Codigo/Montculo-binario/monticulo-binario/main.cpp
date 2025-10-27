#include<iostream>
#include<vector>
using namespace std;
struct registro {
	int edad; 
	string nombre;
	string apellido;
	string diagnostico;
	int nivel_emergencia;
};
class heap {
	vector<registro> cola;
public: 
	heap() {};
	~heap() {};
	void insertar() {
		registro r; 
		cout << "ingrese el nombre" << endl;
		cin >> r.nombre;
		cout << "ingrese el apellido" << endl;
		cin >> r.apellido;
		cout << "diagnostico" << endl;
		cin >> r.diagnostico;
		cout << "Ingrese el grado de emergencia" << endl;
		cin >> r.nivel_emergencia; 
		cola.push_back(r);
	}
	void mostrar() {
		for (int i = 0; i < cola.size(); i++) {
			cout << "Nombre: " << cola[i].nombre << endl;
			cout << "Apellido: " << cola[i].apellido << endl;
			cout << "Diagnostico: " << cola[i].diagnostico << endl;
			cout << "Nivel de emergencia: " << cola[i].nivel_emergencia << endl;
			cout << "------------------------" << endl;	}}
};
void menu(heap& h) {
	int opcion;
	do {
		cout << "menu de opciones" << endl;
		cout << "1.Ingresar paciente" << endl;
		cout << "2.Mostrar pacientes" << endl;
		cout << "2.Atender paciente" << endl;
		cout << "3.Mostrar tiempo de espera" << endl;
		cin >> opcion;
		switch (opcion) {
		case 1:
			h.insertar();
			break;
		case 2:
			h.mostrar();
			break;
		case 3:

			break;
		case 4:
			cout << "Saliendo..." << endl;
			break;
		default:
			cout << "Opción inválida." << endl;
			break;
		}
	} while (opcion != 5);
}

int main() {
	heap h;
	menu(h);

}