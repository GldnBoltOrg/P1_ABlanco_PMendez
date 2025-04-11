//
// Created by Xpc on 23/3/2025.
//
#ifndef MPOINTER_H
#define MPOINTER_H

#include <winsock2.h>
#include <string>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

template<typename T>
class MPointer {
private:
    static SOCKET socketFD;
    static bool initialized;
    int id;

    static std::string sendCommand(const std::string& command);
    static int extractIdFromCreate(const std::string& response);
    static T extractValueFromGet(const std::string& response);

public:
    static void Init(const std::string& ip, int port);

    static MPointer<T> New();

    void operator=(const MPointer<T>& other);

    void operator=(T value);

    T operator*() const;

    int operator&() const;

    ~MPointer();
};

#include "MPointer.cpp"

#endif