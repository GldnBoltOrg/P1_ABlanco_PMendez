//
// Created by Xpc on 11/4/2025.
//

#pragma once

class Node {
public:
    int value;
    MPointer<Node> next;

    Node() : value(0) {} // constructor vacío necesario para instanciación remota
    Node(int value) : value(value) {}
};

