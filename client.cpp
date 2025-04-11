#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: ./client IP PORT\n";
        return 1;
    }

    const char* serverIP = argv[1];
    int serverPort = std::stoi(argv[2]);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creando el socket\n";
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP, &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error conectando al servidor\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Conectado al servidor\n";
    std::cout << "Comandos: \n" <<
                 "1. CREATE <size>\n" <<
                 "2. SET <id> <value>\n" <<
                 "3. GET <id>\n" <<
                 "4. DELETE <id>\n" <<
                 "5. EXIT\n";;

    std::string message;
    char buffer[1024];
    while (true) {
        std::cout << "Mensaje: ";
        std::getline(std::cin, message);
        if (message == "exit") break;

        send(clientSocket, message.c_str(), message.size(), 0);

        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break;

        std::cout << "Servidor: " << buffer << "\n";
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
