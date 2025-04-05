#include <typeindex>
#include "MemoryManager.cpp"

int main() {
    MemoryManager mem(1024);

    int id1 = mem.create(sizeof(int));
    int id2 = mem.create(sizeof(float));

    mem.set(id1, 100);
    mem.set(id2, 3.14f);
}
