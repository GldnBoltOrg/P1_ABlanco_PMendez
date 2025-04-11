#include <typeindex>
#include "MemoryManager.cpp"
#include <iostream>
#include "MPointer.h"

int main() {
    MPointer<int>::Init("127.0.0.1", 5000);  // Dirección y puerto del servidor

    std::cout << "Creando MPointer<int> remoto...\n";
    MPointer<int> miEntero = MPointer<int>::New();

    std::cout << "Asignando valor 123 al puntero remoto...\n";
    miEntero = 123;

    std::cout << "Recuperando valor...\n";
    int valor = *miEntero;

    std::cout << "Valor recuperado: " << valor << std::endl;

    return 0;  // Destructor de miEntero enviará DELETE automáticamente
}


