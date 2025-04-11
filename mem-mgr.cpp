#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <sstream>
#include <map>
#include "MemoryManager.cpp"

#pragma comment(lib, "ws2_32.lib")

void usage() {
    std::cout << "Uso: ./mem-mgr –port LISTEN_PORT –memsize SIZE_MB –dumpFolder DUMP_FOLDER\n";
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        usage();
        return 1;
    }

    int port = 0;
    int memSizeMB = 0;
    std::string dumpFolder;

    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-port") == 0) {
            port = std::stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-memsize") == 0) {
            memSizeMB = std::stoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-dumpFolder") == 0) {
            dumpFolder = argv[i + 1];
        }
    }

    if (port == 0 || memSizeMB == 0 || dumpFolder.empty()) {
        usage();
        return 1;
    }

    std::cout << "Iniciando servidor en el puerto " << port << "...\n";
    std::cout << "Asignando " << memSizeMB << "MB de memoria total\n";

    size_t totalMem = memSizeMB * 1024 * 1024;
    MemoryManager memoryManager(totalMem, dumpFolder);
    std::map<int, int> allocatedBlocks;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket del servidor\n";
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error en bind()\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error en listen()\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Esperando conexiones...\n";

    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error en accept()\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Cliente conectado\n";

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        std::string command(buffer);
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        std::string response;
        if (cmd == "CREATE") {
            int size;
            iss >> size;
            int id = memoryManager.create(size);
            if (id != -1) {
                allocatedBlocks[id] = 0;
                response = "CREATED " + std::to_string(id) + "\n";
            } else {
                response = "ERROR: No hay suficiente memoria\n";
            }
        }
        else if (cmd == "SET") {
            int id, value;
            iss >> id >> value;
            if (allocatedBlocks.find(id) != allocatedBlocks.end()) {
                memoryManager.set(id, value);
                response = "SET OK\n";
            } else {
                response = "ERROR: ID no encontrado\n";
            }
        }
        else if (cmd == "GET") {
            int id;
            iss >> id;
            if (allocatedBlocks.find(id) != allocatedBlocks.end()) {
                int value = memoryManager.get(id);
                response = "VALUE " + std::to_string(value) + "\n";
            } else {
                response = "ERROR: ID no encontrado\n";
            }
        }
        else if (cmd == "DELETE") {
            int id;
            iss >> id;
            if (allocatedBlocks.find(id) != allocatedBlocks.end()) {
                memoryManager.decreaseRefCount(id);
                allocatedBlocks.erase(id);
                response = "DELETED\n";
            } else {
                response = "ERROR: ID no encontrado\n";
            }
        }
        else if (cmd == "INCR") {
    		int id;
    		iss >> id;
    		if (allocatedBlocks.find(id) != allocatedBlocks.end()) {
        		memoryManager.increaseRefCount(id);
        		response = "INCR OK\n";
    	} else {
        	response = "ERROR: ID no encontrado\n";
    	}
	}

        else {
            response = "ERROR: Comando no reconocido\n";
        }

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    std::cout << "Cliente desconectado.\n";

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
