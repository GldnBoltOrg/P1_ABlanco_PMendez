#include <iostream>
#include "MemoryManager.cpp"


int main() {
    MemoryManager memory_manager(1024);
    int id1 = memory_manager.create(sizeof(int), nullptr);
    memory_manager.set(id1, 42);
    std::cout << memory_manager.get(id1) << std::endl;

}