#include <typeindex>
#include "MemoryManager.cpp"
#include <iostream>
#include "MPointer.h"
#include <string>

#include <iostream>
#include <string>
#include "MPointer.h"
#include "Lista.h"

int main() {
    std::string ip;
    int port;

    std::cout << "Ingrese IP del servidor (ej: 127.0.0.1): ";
    std::cin >> ip;

    std::cout << "Ingrese puerto del servidor (ej: 5000): ";
    std::cin >> port;

    MPointer<int>::Init(ip, port);

    static MPointer<int> ptr;
    Lista lista;

    while (true) {
        std::cout << "\n--- Menú Principal ---\n";
        std::cout << "1. Crear nuevo MPointer<int>\n";
        std::cout << "2. Asignar valor\n";
        std::cout << "3. Leer valor\n";
        std::cout << "4. Probar lista enlazada\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opción: ";

        int opcion;
        std::cin >> opcion;

        if (opcion == 1) {
            ptr = MPointer<int>::New();
            std::cout << "MPointer creado con ID remoto: " << &ptr << "\n";
        }
        else if (opcion == 2) {
            int valor;
            std::cout << "Ingrese un valor a asignar: ";
            std::cin >> valor;
            ptr = valor;
            std::cout << "Valor asignado.\n";
        }
        else if (opcion == 3) {
            int valor = *ptr;
            std::cout << "Valor leído desde el servidor: " << valor << "\n";
        }
        else if (opcion == 4) {
            int subopcion;
            std::cout << "\n--- Lista enlazada ---\n";
            std::cout << "1. Agregar elemento\n";
            std::cout << "2. Eliminar elemento\n";
            std::cout << "3. Verificar si contiene\n";
            std::cout << "4. Obtener tamaño\n";
            std::cout << "5. Limpiar lista\n";
            std::cout << "Seleccione una opción: ";
            std::cin >> subopcion;

            if (subopcion == 1) {
                int val;
                std::cout << "Ingrese valor a agregar: ";
                std::cin >> val;
                lista.add(val);
                std::cout << "Agregado.\n";
            }
            else if (subopcion == 2) {
                int val;
                std::cout << "Ingrese valor a eliminar: ";
                std::cin >> val;
                try {
                    lista.remove(val);
                    std::cout << "Eliminado.\n";
                } catch (std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
            }
            else if (subopcion == 3) {
                int val;
                std::cout << "Valor a buscar: ";
                std::cin >> val;
                std::cout << (lista.contains(val) ? "Sí está.\n" : "No está.\n");
            }
            else if (subopcion == 4) {
                std::cout << "Tamaño actual: " << lista.getSize() << "\n";
            }
            else if (subopcion == 5) {
                lista.clear();
                std::cout << "Lista vaciada.\n";
            }
            else {
                std::cout << "Opción inválida en submenú.\n";
            }
        }
        else if (opcion == 5) {
            std::cout << "Saliendo...\n";
            break;
        }
        else {
            std::cout << "Opción inválida.\n";
        }
    }

    return 0;
}




