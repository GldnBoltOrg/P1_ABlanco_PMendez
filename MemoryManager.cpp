#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <vector>

class MemoryManager {
private:
    size_t total_size;
    void* memory_block;

    struct Block {
        size_t size;
        int refCount;
        void* address;
    };

    std::unordered_map<int, Block> memory_map;
    size_t used_memory = 0;

public:
    explicit MemoryManager(size_t size) {
        total_size = size;
        memory_block = malloc(total_size);
        std::cout << "Memory block allocated at " << memory_block << " with size " << total_size << " bytes\n";
    }

    int create(size_t size) {
        if (used_memory + size > total_size) {
            std::cerr << "Error: Not enough memory\n";
            return -1;
        }

        int new_id = memory_map.size();
        void* address = static_cast<char*>(memory_block) + used_memory;

        memory_map[new_id] = {size, 1, address};
        used_memory += size;

        std::cout << "Block " << new_id << " at address " << address << " with size " << size << " bytes\n";
        return new_id;
    }

    template<typename T>
    void set(int id, T value) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: Invalid block ID\n";
            return;
        }

        Block& block = memory_map[id];

        if (sizeof(T) > block.size) {
            std::cerr << "Error: Value size exceeds block size\n";
            return;
        }

        T* ptr = static_cast<T*>(block.address);
        *ptr = value;

        std::cout << "Value set at block " << id << ": " << *ptr << "\n";
    }

    template<typename T>
    T get(int id) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: Invalid block ID\n";
            return T();  // Devuelve un valor predeterminado de T
        }

        Block& block = memory_map[id];

        T* ptr = static_cast<T*>(block.address);
        return *ptr;
    }

    void increaseRefCount(int id) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: Invalid block ID\n";
            return;
        }

        memory_map[id].refCount++;
        std::cout << "Reference count increased for block " << id << ": " << memory_map[id].refCount << "\n";
    }

    void decreaseRefCount(int id) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: Invalid block ID\n";
            return;
        }
        if (memory_map[id].refCount <= 0) {
            std::cerr << "Error: Reference count already zero for block " << id << "\n";
            return;
        }

        memory_map[id].refCount--;
        std::cout << "Reference count decreased for block " << id << ": " << memory_map[id].refCount << "\n";
    }

    void collect_garbage() {
        for (auto it = memory_map.begin(); it != memory_map.end(); ) {
            if (it->second.refCount == 0) {
                std::cout << "Garbage collecting block " << it->first << "\n";
                it = memory_map.erase(it);  // Eliminar el bloque
            } else {
                ++it;
            }
        }
    }

    void defragment() {
    }
};
