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

    while (true) {
        std::cout << "\n--- Menú MPointer ---\n";
        std::cout << "1. Crear nuevo MPointer<int>\n";
        std::cout << "2. Asignar valor\n";
        std::cout << "3. Leer valor\n";
        std::cout << "4. Salir\n";
        std::cout << "Seleccione una opción: ";

        int opcion;
        std::cin >> opcion;

        static MPointer<int> ptr;

        if (opcion == 1) {
            ptr = MPointer<int>::New();
            std::cout << "MPointer creado con ID remoto: " << &ptr << "\n";
        } else if (opcion == 2) {
            int valor;
            std::cout << "Ingrese un valor a asignar: ";
            std::cin >> valor;
            ptr = valor;
            std::cout << "Valor asignado.\n";
        } else if (opcion == 3) {
            int valor = *ptr;
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



