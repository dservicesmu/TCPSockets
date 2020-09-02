// Network.cpp : Defines the functions for the static library.
//


#include "Network.h"
#include <exception>
#include <strstream>

void Network::initialize()
{
    // Initalize winsock
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 2);
    int iResult = WSAStartup(wVersion, &wsaData);
    if (iResult != 0)
    {
        std::ostrstream ostr;
        ostr << "Winsock startup failed, error = " << iResult << std::endl;
        throw std::runtime_error(ostr.str());
    }
}

void Network::uninitialize()
{
    int iResult = WSACleanup();
    if (iResult == SOCKET_ERROR)
    {
        std::ostrstream ostr;
        ostr << "Winsock cleanup failed, error = " << WSAGetLastError() << std::endl;
        throw std::runtime_error(ostr.str());
    }
}


std::shared_ptr<TCPServer>& Network::createTCPServer()
{
    m_tcpServers.emplace_back(std::shared_ptr<TCPServer>(new TCPServer()));
    return m_tcpServers.back();
}
