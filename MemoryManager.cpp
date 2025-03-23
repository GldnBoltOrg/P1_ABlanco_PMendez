#include <algorithm>
#include <iostream>
#include <map>
#include <cstring>
#include <unordered_map>
#include <vector>

class MemoryManager {
private:
    size_t total_size;
    size_t used_size = 0;
    int current_id = 0;
    void* memory_block;
    std::unordered_map<size_t, std::pair<void*, int>> memory_map; // id -> (direccion, ref count)

public:
    explicit MemoryManager(size_t size) {
        total_size = size;
        memory_block = malloc(total_size);
        std::cout << "Memory block allocated at " << memory_block << std::endl;
    }

    int create(size_t size, void* type) {
        if (used_size + size > total_size) return -1;
        void* new_memory = static_cast<char*>(memory_block) + used_size;
        memory_map.insert({used_size, {new_memory, 1}});
        used_size += size;
        return current_id++;
    }

    bool set(int id, int value) {
        if (memory_map.find(id) == memory_map.end()) return false;
        *static_cast<int*>(memory_map[id].first) = value;
        return true;
    }

    int get(int id) {
        if (memory_map.find(id) == memory_map.end()) return -1;
        return *static_cast<int*>(memory_map[id].first);
    }

    void increaseRefCount(int id) {
        if (memory_map.find(id) != memory_map.end()) {
            memory_map[id].second++;
        }
    }

    void decreaseRefCount(int id) {
        if (memory_map.find(id) != memory_map.end()) {
            if (--memory_map[id].second == 0) {
                memory_map.erase(id);
            }
        }
    }
};

