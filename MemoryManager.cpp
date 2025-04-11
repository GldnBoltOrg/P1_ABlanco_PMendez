#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <iomanip>
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
        std::cout << "Bloque de memoria ubicado en: " << memory_block << ", de tamano:" << total_size << " bytes\n";
    }

    int create(size_t size) {
        if (used_memory + size > total_size) {
            std::cerr << "Error: No hay suficiente memoria \n";
            return -1;
        }

        int new_id = memory_map.size();
        void* address = static_cast<char*>(memory_block) + used_memory;

        memory_map[new_id] = {size, 1, address};
        used_memory += size;

        std::cout << "Se creo bloque #" << new_id << " en la direccion: " << address << ", de tamano:" << size << " bytes\n";
        return new_id;
    }

    template<typename T>
    bool set(int id, T value) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: ID ivalido\n";
            return false;
        }

        Block& block = memory_map[id];

        if (sizeof(T) > block.size) {
            std::cerr << "Error: El valor es mayor al tamaño del bloque\n";
            return false;
        }

        T* ptr = static_cast<T*>(block.address);
        *ptr = value;
        return true;

        create_dump();
    }

    int get(int id) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: ID ivalido\n";
            return -1;
        }

        Block& block = memory_map[id];

        int* ptr = static_cast<int*>(block.address);
        increaseRefCount(id);
        return *ptr;
    }

    void increaseRefCount(int id) {
        memory_map[id].refCount++;
        create_dump();
    }

    void decreaseRefCount(int id) {
        if (memory_map.find(id) == memory_map.end()) {
            std::cerr << "Error: ID ivalido\n";
            return;
        }
        if (memory_map[id].refCount <= 0) {
            std::cerr << "Error: El bloque ya tiene 0 referencias" << id << "\n";
            return;
        }

        memory_map[id].refCount--;
        collect_garbage();
    }

    void collect_garbage() {
        for (auto it = memory_map.begin(); it != memory_map.end(); ) {
            if (it->second.refCount == 0) {
                it = memory_map.erase(it);
            } else {
                ++it;
            }
        }
        defragment();
        create_dump();
    }

    void defragment() {
        std::vector<std::pair<int, Block>> used_blocks;
        for (const auto& [id, block] : memory_map) {
            if (block.refCount > 0) {
                used_blocks.push_back({id, block});
            }
        }

        std::sort(used_blocks.begin(), used_blocks.end(), [](const auto& a, const auto& b) {
            return a.second.address < b.second.address;
        });

        size_t offset = 0;

        for (auto& [id, block] : used_blocks) {
            void* new_address = static_cast<char*>(memory_block) + offset;

            if (block.address != new_address) {
                char* src = static_cast<char*>(block.address);
                char* dest = static_cast<char*>(new_address);

                for (size_t i = 0; i < block.size; ++i) {
                    dest[i] = src[i];
                }

                block.address = new_address;
            }

            memory_map[id] = block;

            offset += block.size;
        }

        used_memory = offset;
    }

    void create_dump() {
        auto now = std::chrono::system_clock::now();
        auto in_time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream filename;
        filename << dump_folder << "/dump_"
                 << std::put_time(std::localtime(&in_time), "%Y%m%d_%H%M%S")
                 << "_" << std::setfill('0') << std::setw(3) << ms.count() << ".txt";

        std::ofstream file(filename.str());
        if (!file.is_open()) {
            std::cerr << "Error creando dump file\n";
            return;
        }

        file << "== Memory Dump ==\n";
        for (const auto& [id, block] : memory_map) {
            file << "ID: " << id
                << " | Address: " << block.address
                << " | Size: " << block.size
                << " | RefCount: " << block.refCount << "\n";
        }
        file.close();
    }

    void* getAddress(int id) {
        if (memory_map.find(id) != memory_map.end()) {
            return memory_map[id].address;
        }
        return nullptr;
    }
};
