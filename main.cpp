#include <typeindex>
#include "MemoryManager.cpp"
#include <iostream>
#include "MPointer.h"
#include <string>

int main() {
    std::string ip;
    int port;

    std::cout << "Ingrese IP del servidor (ej: 127.0.0.1): ";
    std::cin >> ip;

    std::cout << "Ingrese puerto del servidor (ej: 5000): ";
    std::cin >> port;

    MPointer<int>::Init(ip, port);

    MPointer<int> ptr;
    int lastId = -1;

    while (true) {
        std::cout << "\n--- Menú MPointer ---\n";
        std::cout << "1. Crear nuevo MPointer<int>\n";
        std::cout << "2. Asignar valor a MPointer actual\n";
        std::cout << "3. Leer valor por ID\n";
        std::cout << "4. Salir\n";
        std::cout << "Seleccione una opción: ";

        int opcion;
        std::cin >> opcion;

        if (opcion == 1) {
            ptr = MPointer<int>::New();
            lastId = &ptr;
            std::cout << "MPointer creado con ID remoto: " << lastId << "\n";
        } else if (opcion == 2) {
            if (lastId == -1) {
                std::cout << "Primero cree un MPointer.\n";
                continue;
            }
            int valor;
            std::cout << "Ingrese un valor a asignar: ";
            std::cin >> valor;
            ptr = valor;
            std::cout << "Valor asignado a MPointer con ID " << lastId << "\n";
        } else if (opcion == 3) {
            int id;
            std::cout << "Ingrese ID del MPointer a leer: ";
            std::cin >> id;

            MPointer<int> temp;
            *((int*)&temp) = id; // Hack para inyectar ID (por simplicidad)
            int valor = *temp;
            std::cout << "Valor leído desde el servidor: " << valor << "\n";
        } else if (opcion == 4) {
            std::cout << "Saliendo...\n";
            break;
        } else {
            std::cout << "Opción inválida.\n";
        }
    }

    return 0;
}



