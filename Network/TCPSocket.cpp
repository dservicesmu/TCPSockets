
#include "TCPSocket.h"
#include <array>
#include <strstream>

TCPSocket::TCPSocket(
    ) : m_bufferSize(256)
      , m_receiveSize(0)
      , m_socket(INVALID_SOCKET)
      , m_bufferPtr(NULL)
{
    m_bufferPtr = new char[m_bufferSize];
}

TCPSocket::TCPSocket(
    SOCKET socket,
    std::size_t bufferSize
    ) : m_bufferSize(bufferSize)
      , m_receiveSize(0)
      , m_socket(socket)
      , m_bufferPtr(NULL)
{
    m_bufferPtr = new char[m_bufferSize];
} 

TCPSocket::~TCPSocket()
{
    m_bufferSize = 0;
    delete[] m_bufferPtr;
}

std::string TCPSocket::getAddress()
{
    std::array<char, 64> addressStr;
    DWORD outLen = 64;

    sockaddr clientAddr;
    int addrSize = sizeof(clientAddr);
    int iResult = getpeername(m_socket, &clientAddr, &addrSize);
    if (iResult == SOCKET_ERROR)
    {
        std::ostrstream msg;
        msg << "Getpeername on socket failed, error = " << WSAGetLastError() << std::ends;
        closesocket(m_socket);
        throw std::runtime_error(msg.str());
    }

	iResult = WSAAddressToStringA(&clientAddr, addrSize, NULL, &addressStr[0], &outLen);
	if (iResult == SOCKET_ERROR)
	{
		std::ostrstream msg;
		msg << "WSAAddressToStringA on socket failed, error = " << WSAGetLastError() << std::ends;
		closesocket(m_socket);
		throw std::runtime_error(msg.str());
	}
	addressStr[outLen] = NULL;
    return std::string(&addressStr[0]);
}

void TCPSocket::send(char const* dataPtr, size_t length)
{
    int iResult = ::send(m_socket, dataPtr, static_cast<int>(length), 0);
	if (iResult == SOCKET_ERROR)
	{
		std::ostrstream msg;
		msg << "Send to clent failed: " << WSAGetLastError() << std::endl;
		closesocket(m_socket);
		throw std::runtime_error(msg.str());
    }
}

TCPData TCPSocket::receive()
{
    int iResult = ::recv(m_socket, m_bufferPtr, static_cast<int>(m_bufferSize), 0);
    if (iResult == SOCKET_ERROR)
    {
		std::ostrstream msg;
		msg << "Call to receive failed, error = " << WSAGetLastError() << std::ends;
		closesocket(m_socket);
		throw std::runtime_error(msg.str());
    }
    return TCPData{ static_cast<std::size_t>(iResult), m_bufferPtr };
}
