#include <typeindex>
#include "MemoryManager.cpp"

#include "MPointer.h"

int main() {
    MPointer<int>::Init("127.0.0.1", 5000);

    MPointer<int> a = MPointer<int>::New();
    a = 42;

    int val = *a;
    std::cout << "Valor almacenado: " << val << std::endl;

    return 0;
}

