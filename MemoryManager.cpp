#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <vector>

class MemoryManager {
private:
    size_t total_size;
    void* memory_block;
    std::string dump_folder;

    struct Block {
        size_t size;
        int refCount;
        void* address;
    };

    std::unordered_map<int, Block> memory_map;
    size_t used_memory = 0;

public:
    explicit MemoryManager(size_t size, std::string folder) {
        total_size = size;
        memory_block = malloc(total_size);
        dump_folder = folder;
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
        create_dump();
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
        create_dump();
    }

    int get(int id) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: Invalid block ID\n";
            return -1;
        }

        Block& block = memory_map[id];

        int* ptr = static_cast<int*>(block.address);
        return *ptr;
        create_dump();
    }

    void increaseRefCount(int id) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: Invalid block ID\n";
            return;
        }

        memory_map[id].refCount++;
        std::cout << "Reference count increased for block " << id << ": " << memory_map[id].refCount << "\n";
        create_dump();
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
        create_dump();
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
        create_dump();
    }

    void defragment() {
        std::cout << "Starting memory defragmentation...\n";

        // Recolectar bloques usados
        std::vector<std::pair<int, Block>> used_blocks;
        for (const auto& [id, block] : memory_map) {
            if (block.refCount > 0) {
                used_blocks.push_back({id, block});
            }
        }

        // Ordenarlos por dirección original para mantener orden
        std::sort(used_blocks.begin(), used_blocks.end(), [](const auto& a, const auto& b) {
            return a.second.address < b.second.address;
        });

        size_t offset = 0;

        for (auto& [id, block] : used_blocks) {
            void* new_address = static_cast<char*>(memory_block) + offset;

            // Solo copiar si se mueve a otra dirección
            if (block.address != new_address) {
                char* src = static_cast<char*>(block.address);
                char* dest = static_cast<char*>(new_address);

                for (size_t i = 0; i < block.size; ++i) {
                    dest[i] = src[i];
                }

                block.address = new_address;
                std::cout << "Block " << id << " moved to address " << new_address << "\n";
            }

            // Actualizar el mapa con la nueva dirección
            memory_map[id] = block;

            offset += block.size;
        }

        used_memory = offset;

        std::cout << "Defragmentation complete. Used memory: " << used_memory << " bytes\n";
        create_dump();
    }

    void create_dump() {
    }

};
