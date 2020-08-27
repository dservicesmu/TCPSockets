// Client

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
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

    // Open the client socket
    struct addrinfo* result = NULL;
    struct addrinfo* ptr = NULL;
    struct addrinfo  hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Get the Server IP address and port
    const char HOSTNAME[] = "MathTixWinDev";
    const char SERVER_PORT[] = "48000";
    iResult = getaddrinfo(HOSTNAME, SERVER_PORT, &hints, &result);
    if (iResult != 0)
    {
        std::cerr << "Call to getaddrinfo() failed" << std::endl;
        WSACleanup();
        return 1;
    }
    ptr = result;

    // Create the socketf
    SOCKET socketToServer = INVALID_SOCKET;
    socketToServer = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (socketToServer == INVALID_SOCKET)
    {
        std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to the server
    iResult = connect(socketToServer, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(socketToServer);
        socketToServer = INVALID_SOCKET;
    }
    freeaddrinfo(result);

    if (socketToServer == INVALID_SOCKET)
    {
        std::cerr << "Server connection failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Send a message to the server
    const char* messageToServer = "Send me some data";
    iResult = send(socketToServer, messageToServer, (int)strlen(messageToServer), 0);
    if (iResult == SOCKET_ERROR)
    {
        std::cerr << "Send message to server failed: " << WSAGetLastError() << std::endl;
        closesocket(socketToServer);
        WSACleanup();
        return 1;
    }

    return 0;
}
