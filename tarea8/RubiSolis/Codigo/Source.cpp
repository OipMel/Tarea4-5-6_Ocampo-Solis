#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
using namespace std;
using sf::Vector2f;
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
	int gettamaño() {
		return cola.size();
	}
	//heapify down
	void reacomodar(int i) {
		int menor = i;
		int hijoIzq = 2 * i + 1;
		int hijoDer = 2 * i + 2;
		if (hijoIzq < gettamaño() && cola[hijoIzq].nivel_emergencia < cola[menor].nivel_emergencia) {
			menor = hijoIzq;
		}
		if (hijoDer < gettamaño() && cola[hijoDer].nivel_emergencia < cola[menor].nivel_emergencia) {
			menor = hijoDer;
		}
		if (menor != i) {
			swap(cola[i], cola[menor]);
			reacomodar(menor);
		}
	}
	void sacar_raiz() {
		registro raiz = cola[0]; //primer elemento
		registro ultimo = cola.back(); //ultimo elemeto 
		cout << "Atendiendo paciente: " << endl;
		cout << "nombre:" << raiz.nombre << endl;
		cout << "nombre:" << raiz.apellido << endl;
		cola[0] = cola.back();
		cola.pop_back(); //elimina el ultimo elemento 
		reacomodar(0);
	}
	void heapmin(registro r) {
		cola.push_back(r);
		int hijo = gettamaño() - 1;
		int padre = (hijo - 1) / 2;
		while (hijo > 0 && cola[hijo].nivel_emergencia < cola[padre].nivel_emergencia) {
			swap(cola[hijo], cola[padre]);
			hijo = padre;
			padre = (hijo - 1) / 2;
		}
	}
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
		heapmin(r);
	}
	void mostrar() {
		for (int i = 0; i < cola.size(); i++) {
			cout << "Nombre: " << cola[i].nombre << endl;
			cout << "Apellido: " << cola[i].apellido << endl;
			cout << "Diagnostico: " << cola[i].diagnostico << endl;
			cout << "Nivel de emergencia: " << cola[i].nivel_emergencia << endl;
			cout << "------------------------" << endl;
		}
	}
};
void menu(heap& h) {
	int opcion;
	do {
		cout << "menu de opciones" << endl;
		cout << "1.Ingresar paciente" << endl;
		cout << "2.Mostrar pacientes" << endl;
		cout << "3.Atender paciente (extraer raiz)" << endl;
		cout << "4.Mostrar SFML" << endl;
		cout << "5.Salir" << endl;
		cin >> opcion;
		switch (opcion) {
		case 1:
			h.insertar();
			break;
		case 2:
			h.mostrar();
			break;
		case 3:
			h.sacar_raiz();
			break;
		case 4:
			cout << "Mostrando..." << endl;

			break;
		case 5:
			cout << "Saliendo del programa" << endl;
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
	sf::RenderWindow window(sf::VideoMode(800, 600), "Prueba SFML");
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.display();
	}
	return 0;

}