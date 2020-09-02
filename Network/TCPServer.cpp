// Server

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <strstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "TCPSocket.h"

#include "TCPServer.h"


void TCPServer::bind(std::uint16_t port)
{
    // Resolve the port locally
    struct addrinfo  addrHintsIn;
    struct addrinfo* pAddrOut = NULL;

    ZeroMemory(&addrHintsIn, sizeof(addrHintsIn));
    addrHintsIn.ai_family = AF_INET;
    addrHintsIn.ai_socktype = SOCK_STREAM;
    addrHintsIn.ai_protocol = IPPROTO_TCP;
    addrHintsIn.ai_flags = AI_PASSIVE;

    // Get the Server IP address and port
    std::ostrstream serverPort;
    serverPort << port << std::ends;
    int iResult = ::getaddrinfo(NULL, serverPort.str(), &addrHintsIn, &pAddrOut);
    if (iResult != 0)
    {
        std::ostrstream msg;
        msg << "Call to getaddrinfo() failed, error = " << iResult << std::ends;
        throw std::runtime_error(msg.str());
    }

    // Create a socket to listen for connectons.
    m_listenSocket = ::socket(pAddrOut->ai_family, pAddrOut->ai_socktype, pAddrOut->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET)
    {
        std::ostrstream msg;
        msg << "Listen socket creation error, error = " << WSAGetLastError() << std::ends;
        freeaddrinfo(pAddrOut);
        throw std::runtime_error(msg.str());
    }

    // Bind the listening socket
    iResult = ::bind(m_listenSocket, pAddrOut->ai_addr, (int)pAddrOut->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        std::ostrstream msg;
        msg << "Sever socket bind failed, error = " << WSAGetLastError() << std::ends;
        freeaddrinfo(pAddrOut);
        closesocket(m_listenSocket);
        throw std::runtime_error(msg.str());
    }
    freeaddrinfo(pAddrOut);
}

void TCPServer::listen()
{
    // Start listening for client connections.
    int iResult = ::listen(m_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        std::ostrstream msg;
        msg << "Listen on server socket failed, error = " << WSAGetLastError() << std::ends;
        closesocket(m_listenSocket);
        throw std::runtime_error(msg.str());
    }
}

TCPSocket TCPServer::accept()
{
    // Accept client connections
    SOCKET socket = INVALID_SOCKET;
    socket = ::accept(m_listenSocket, NULL, NULL);
    if (socket == INVALID_SOCKET)
    {
        std::ostrstream msg;
        msg << "Accept on server socket failed, error = " << WSAGetLastError() << std::ends;
        closesocket(m_listenSocket);
        throw std::runtime_error(msg.str());
    }
    return TCPSocket(socket);
}
