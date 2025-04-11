//
// Created by Xpc on 11/4/2025.
//

#include "Lista.h"
#pragma once
#include <stdexcept>
#include "MPointer.h"
#include "Node.h"

class Lista {
private:
    MPointer<Node> head;
    int size;

public:
    Lista() {
        size = 0;
    }

    void clear() {
        head = MPointer<Node>();  // vacía
        size = 0;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }

    bool contains(int element) {
        MPointer<Node> current = head;
        while (&current != 0) {
            if ((*current).value == element) return true;
            current = (*current).next;
        }
        return false;
    }

    bool add(int element) {
        MPointer<Node> newNode = MPointer<Node>::New();
        (*newNode).value = element;

        if (&head == 0) {
            head = newNode;
        } else {
            MPointer<Node> current = head;
            while (&(*current).next != 0) {
                current = (*current).next;
            }
            (*current).next = newNode;
        }
        size++;
        return true;
    }

    int remove(int element) {
        if (&head == 0) {
            throw std::runtime_error("Lista vacía");
        }

        if ((*head).value == element) {
            head = (*head).next;
            size--;
            return element;
        }

        MPointer<Node> current = head;
        while (&(*current).next != 0) {
            if ((*(*current).next).value == element) {
                (*current).next = (*(*current).next).next;
                size--;
                return element;
            }
            current = (*current).next;
        }

        throw std::runtime_error("Elemento no encontrado");
    }
};
