// Client

#include <TCPClient.h>
#include <iostream>
#include <strstream>
#include <array>

TCPSocket TCPClient::connect(
	std::string const& host,
	std::uint16_t port
	)
{
	 // Resolve the port locally
    struct addrinfo  addrHintsIn;
    struct addrinfo* pAddrOut = NULL;

    ZeroMemory(&addrHintsIn, sizeof(addrHintsIn));
    addrHintsIn.ai_family = AF_INET;
    addrHintsIn.ai_socktype = SOCK_STREAM;
    addrHintsIn.ai_protocol = IPPROTO_TCP;

    // Get the Server IP address and port
    std::ostrstream serverPort;
    serverPort << port << std::ends;
    int iResult = ::getaddrinfo(host.c_str(), serverPort.str(), &addrHintsIn, &pAddrOut);
    if (iResult != 0)
    {
        std::ostrstream msg;
        msg << "Call to getaddrinfo() failed, error = " << iResult << std::ends;
        throw std::runtime_error(msg.str());
    }

    // Create the connection socket.
    SOCKET socket = ::socket(pAddrOut->ai_family, pAddrOut->ai_socktype, pAddrOut->ai_protocol);
    if (socket == INVALID_SOCKET)
    {
        std::ostrstream msg;
        msg << "Connection socket creation error, error = " << WSAGetLastError() << std::ends;
        freeaddrinfo(pAddrOut);
        throw std::runtime_error(msg.str());
    }

     // Connect to the server
    iResult = ::connect(socket, pAddrOut->ai_addr, (int)pAddrOut->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(socket);
        socket = INVALID_SOCKET;
    }
    freeaddrinfo(pAddrOut);

    if (socket == INVALID_SOCKET)
    {
        std::ostrstream msg;
        msg << "Connection failed" << std::ends;
        throw std::runtime_error(msg.str());
    }
    return TCPSocket(socket);
}
