#include <iostream>
#include <vector>
using namespace std;

void swap(int* x, int* y){
	int temp = *x;
	*x = *y;
	*y = temp;
}; //intercambio de dos variables

class Heap {
	vector<int> arr;
	bool isMinHeap;

public:
	Heap(bool minHeap = true) { isMinHeap = minHeap; }

	//Esta funcion lo que hace es reorganizar el vector para que cumpla con las propiedades del heap
	void insertar(int val) {  
		arr.push_back(val);
		int i = arr.size() - 1;
		while (i > 0) {
			int padre = (i - 1) / 2;
			if (isMinHeap ? arr[i]<arr[padre] : arr[i]>arr[padre]) {
				swap(arr[i], arr[padre]);
				i = padre;
			}
			else break;
		}
	}
	//Esta funcion permite la insercion de nodos, cuantos el usuario desee.
	void insertarNodos() {
		char resp;
		int val;

		do {
			cout << "Ingrese valor a insertar: ";
			cin >> val;
			insertar(val);  // Llama al método que inserta en el heap

			// Validar la respuesta del usuario
			do {
				cout << "Desea ingresar otro nodo? (s/n): ";
				cin >> resp;

				// Convertir a minúscula para simplificar la comparación
				resp = tolower(resp);

				if (resp != 's' && resp != 'n') {
					cout << "Error: solo puede responder con 's' o 'n'. Intente de nuevo.\n";
				}
			} while (resp != 's' && resp != 'n');

		} while (resp == 's');

		cout << "Volviendo al menu principal...\n";
	}

	void mostrar() {
		if (arr.empty()) {
			cout << "El vector esta vacio. Ingrese los nodos necesarios" << endl;
		}
		else {
			cout << "Heap actual: ";
			for (int val : arr) {
				cout << val << " ";
			}
			cout << endl;
		}
	}

	int mostrarRaiz() {
		if (!arr.empty()) return arr[0];
		throw runtime_error("Heap vacío");
	}

	//int extraerRaiz() {
	//	if (arr.empty()) throw runtime_error("Heap vacío");
	//	int root = arr[0];
	//	arr[0] = arr.back();
	//	arr.pop_back();
	//	heapify(0);
	//	return root;
	//}

};	
 
void ConsultarRaiz() {
	//funcion consultar raiz
};


int main() {
	Heap heap;
	int opcion;
	int n;
	cout << "\nMenu principal - informacion adicional: \n" << endl;
	cout << "1. Insertar nodos: " << endl;
	cout << "2. Consultar raiz " << endl; //Mostrar el elemento en la raiz. Si el heap esta vacio, mensaje informativo
	cout << "3. Extraer raiz " << endl;//Extrae y muestra el valor extraido y el nuevo estado del heap
	cout << "4. Heapify " << endl; //Pedir lista de enteros. 
	cout << "5. Alternar Min/Max" << endl; //opcional
	cout << "6. Mostrar vector subadyacente" << endl; //imprime el std::vector
	cout << "7. Simulacion con restaurante" << endl;
	cout << "8. Salir" << endl;
	cout << "Ingrese una opcion: ";
	cin >> opcion;


	switch (opcion) {
	case 1:
		heap.insertarNodos();//funcion insertar nodos
		break;
	case 2:
		//funcion consultar raiz
		cout << "---Bienvenido---" << endl;
		cout << "Consultar raiz: " << endl;
		break;
	case 3:
		//funcion extraer raiz
		cout << "---Bienvenido---" << endl;
		cout << "Extraer raiz: " << endl;
		break;
	case 4:
		//funcion heapify
		cout << "---Bienvenido---" << endl;
		cout << "Heapify: " << endl;
		break;
	case 5:
		//funcion alternar min/max
		cout << "---Bienvenido---" << endl;
		cout << "Alternar Min/Max: " << endl;
		break;
	case 6:
		heap.mostrar();//funcion mostrar vector subadyacente
		break;
	case 7:
		//funcion simulacion con restaurante
		cout << "---Bienvenido---" << endl;
		cout << "Simulacion con restaurante: " << endl;
		break;
	case 8:
		//salir
		cout << "Salir" << endl;
		break;
	default:
		cout << "Opcion no valida, intente de nuevo." << endl;
		break;

	} while (opcion != 8);
	return 0;
};