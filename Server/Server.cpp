// Server

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <string>
#include <array>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    // Initalize winsock
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 2);
    int iResult = WSAStartup(wVersion, &wsaData);
    if (iResult != 0)
    {
        std::cerr << "Winsock startup failed" << std::endl;
        return 1;
    }

    // Resolve the port locally
    struct addrinfo  addrHintsIn;
    struct addrinfo* pAddrOut = NULL;

    ZeroMemory(&addrHintsIn, sizeof(addrHintsIn));
    addrHintsIn.ai_family = AF_INET;
    addrHintsIn.ai_socktype = SOCK_STREAM;
    addrHintsIn.ai_protocol = IPPROTO_TCP;
    addrHintsIn.ai_flags = AI_PASSIVE;

    // Get the Server IP address and port
    std::string serverPort("48000");
    iResult = getaddrinfo(NULL, serverPort.c_str(), &addrHintsIn, &pAddrOut);
    if (iResult != 0)
    {
        std::cerr << "Call to getaddrinfo() failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Create a socket to listen for connectons.
    SOCKET listenSocket = INVALID_SOCKET;
    listenSocket = socket(pAddrOut->ai_family, pAddrOut->ai_socktype, pAddrOut->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Listen socket creation error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(pAddrOut);
        WSACleanup();
        return 1;
    }

    // Bind the listening socket
    iResult = bind(listenSocket, pAddrOut->ai_addr, (int)pAddrOut->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        std::cerr << "Sever socket bind failed: " << WSAGetLastError() << std::endl;
        freeaddrinfo(pAddrOut);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(pAddrOut);

    // Start listening for client connections.
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        std::cerr << "Listen on server socket failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Server is listening for client connections" << std::endl;

    // Accept client connections
    SOCKET clientSocket = INVALID_SOCKET;
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Read data from the client
    std::array<char, 512> receiveBuffer;
    iResult = recv(clientSocket, &(receiveBuffer[0]), receiveBuffer.size(), 0);
    if (iResult < receiveBuffer.size())
    {
        receiveBuffer[iResult] = NULL;
        std::cout << "Received message from client!" << std::endl
            << "\tMessage length: " << iResult << std::endl
            << "\tMessage: " << std::string(&receiveBuffer[0]) << std::endl;
    }
    else if (iResult == 0)
    {
        std::cout << "Connection is closing" << std::endl;
    }
    else
    {
        std::cerr << "Receive from client failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        return 1;
    }

    iResult = shutdown(clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        std::cerr << "Clent socket shutdown failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
