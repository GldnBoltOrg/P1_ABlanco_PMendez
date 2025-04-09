#include <typeindex>
#include "MemoryManager.cpp"

int main() {
    MemoryManager manager(1024, "hola"); // 1 KB de memoria para pruebas

    int id1 = manager.create(100);
    manager.set<int>(id1, 42);

    int id2 = manager.create(200);
    manager.set<int>(id2, 84);

    int id3 = manager.create(150);
    manager.set<int>(id3, 168);

    std::cout << "\n== Estado inicial de memoria ==\n";
    std::cout << "ID 1 dirección: " << manager.getAddress(id1) << "\n";
    std::cout << "ID 2 dirección: " << manager.getAddress(id2) << "\n";
    std::cout << "ID 3 dirección: " << manager.getAddress(id3) << "\n";

    std::cout << "ID 1 value: " << manager.get(id1) << "\n";
    std::cout << "ID 2 value: " << manager.get(id2) << "\n";
    std::cout << "ID 3 value: " << manager.get(id3) << "\n";

    // Liberar el bloque del medio
    manager.decreaseRefCount(id2);
    manager.collect_garbage();

    std::cout << "\n== Después de garbage collection ==\n";
    std::cout << "ID 1 dirección: " << manager.getAddress(id1) << "\n";
    std::cout << "ID 3 dirección: " << manager.getAddress(id3) << "\n";

    // Defragmentar
    manager.defragment();

    std::cout << "\n== Después de defragmentación ==\n";
    std::cout << "ID 1 dirección: " << manager.getAddress(id1) << "\n";
    std::cout << "ID 3 dirección: " << manager.getAddress(id3) << "\n";

    return 0;
}

