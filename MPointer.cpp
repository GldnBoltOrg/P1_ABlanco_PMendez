//
// Created by Xpc on 23/3/2025.
//
#include "MPointer.h"
#include <iostream>
#include <sstream>
#include <ws2tcpip.h>

template<typename T>
SOCKET MPointer<T>::socketFD;

template<typename T>
bool MPointer<T>::initialized = false;

template<typename T>
void MPointer<T>::Init(const std::string& ip, int port) {
    if (initialized) return;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == INVALID_SOCKET) {
        std::cerr << "Error creando socket\n";
        exit(1);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(socketFD, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error conectando al servidor\n";
        closesocket(socketFD);
        WSACleanup();
        exit(1);
    }

    initialized = true;
}

template<typename T>
std::string MPointer<T>::sendCommand(const std::string& command) {
    send(socketFD, command.c_str(), command.size(), 0);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(socketFD, buffer, sizeof(buffer), 0);
    return std::string(buffer);
}

template<typename T>
int MPointer<T>::extractIdFromCreate(const std::string& response) {
    std::istringstream iss(response);
    std::string token;
    iss >> token; // CREATED
    int id;
    iss >> id;
    return id;
}

template<typename T>
T MPointer<T>::extractValueFromGet(const std::string& response) {
    std::istringstream iss(response);
    std::string token;
    T value;
    iss >> token >> value;
    return value;
}

template<typename T>
MPointer<T> MPointer<T>::New() {
    std::string response = sendCommand("CREATE " + std::to_string(sizeof(T)));
    MPointer<T> ptr;
    ptr.id = extractIdFromCreate(response);
    return ptr;
}

template<typename T>
void MPointer<T>::operator=(const MPointer<T>& other) {
    id = other.id;
    sendCommand("INCR " + std::to_string(id));
}

template<typename T>
void MPointer<T>::operator=(T value) {
    sendCommand("SET " + std::to_string(id) + " " + std::to_string(value));
}

template<typename T>
T MPointer<T>::operator*() const {
    std::string response = sendCommand("GET " + std::to_string(id));
    return extractValueFromGet(response);
}

template<typename T>
int MPointer<T>::operator&() const {
    return id;
}

template<typename T>
MPointer<T>::~MPointer() {
    sendCommand("DELETE " + std::to_string(id));
}
