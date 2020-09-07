
#include <TCPSocket.h>
#include <limits>
#include <array>
#include <strstream>
#include <iostream>

#ifdef max
#undef max
#endif

TCPSocket::TCPSocket(
    ) : m_mode(Mode::Blocking)
      , m_socket(INVALID_SOCKET)
      , m_timeval{0l, 0l}
      , m_bufferSize(256)
      , m_receiveSize(0)
      , m_bufferPtr(NULL)
{
    FD_ZERO(&m_fdSet);
    m_bufferPtr = new char[m_bufferSize];
}

TCPSocket::TCPSocket(
    SOCKET socket,
    Mode mode,
    std::size_t bufferSize
    ) : m_mode(mode)
      , m_socket(socket)
      , m_timeval{0l, 0l}
      , m_bufferSize(bufferSize)
      , m_receiveSize(0)
      , m_bufferPtr(NULL)
{
    FD_ZERO(&m_fdSet);
    m_bufferPtr = new char[m_bufferSize];
} 

TCPSocket::TCPSocket(
    const TCPSocket& src
    ) : m_mode(src.m_mode)
      , m_socket(src.m_socket)
      , m_fdSet(src.m_fdSet)
      , m_timeval(src.m_timeval)
      , m_bufferSize(src.m_bufferSize)
      , m_receiveSize(0)
      , m_bufferPtr(NULL)
{
    if (m_bufferSize > 0)
    {
        m_bufferPtr = new char[m_bufferSize];
    }
}

TCPSocket::~TCPSocket()
{
    m_mode = Mode::Invalid;
    m_socket = INVALID_SOCKET;
    FD_ZERO(&m_fdSet);
    m_bufferSize = 0;
    m_receiveSize = 0;
    delete[] m_bufferPtr;
}

TCPSocket& TCPSocket::operator=(const TCPSocket& src)
{
    if (this != &src)
    {
        m_mode = src.m_mode;
        m_bufferSize = src.m_bufferSize;
        m_receiveSize = src.m_receiveSize;
        m_socket = src.m_socket;
        if (m_bufferSize > 0)
        {
            m_bufferPtr = new char[m_bufferSize];
        }
    }
    return *this;
}

Mode TCPSocket::getMode()
{
    return m_mode;
}

void TCPSocket::setMode(Mode mode)
{
    m_mode = mode;
    if (m_mode != Mode::Invalid)
    {
        u_long winsockmode = 0;
        switch (m_mode)
        {
        case Mode::Blocking: winsockmode = 0; break;
        case Mode::Nonblocking: winsockmode = 1; break;
        }
        int iResult = ioctlsocket(m_socket, FIONBIO, &winsockmode);
		if (iResult == SOCKET_ERROR)
		{
			std::ostrstream msg;
			msg << "Call to ioctlsocket on socket failed, error = " << WSAGetLastError() << std::ends;
			throw std::runtime_error(msg.str());
		}
    }

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
        throw std::runtime_error(msg.str());
    }

	iResult = WSAAddressToStringA(&clientAddr, addrSize, NULL, &addressStr[0], &outLen);
	if (iResult == SOCKET_ERROR)
	{
		std::ostrstream msg;
		msg << "WSAAddressToStringA on socket failed, error = " << WSAGetLastError() << std::ends;
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
		msg << "Send on socket failed: " << WSAGetLastError() << std::ends;
		closesocket(m_socket);
		throw std::runtime_error(msg.str());
    }
    else if (iResult < static_cast<int>(length))
    {
		std::ostrstream msg;
		msg << "Requested (" << length << ") bytes to be sent. "
            << "Only (" << iResult << ") bytes were sent."
            << std::ends;
		closesocket(m_socket);
		throw std::runtime_error(msg.str());
    }
}

TCPData TCPSocket::receive()
{
    int iResult = ::recv(m_socket, m_bufferPtr, static_cast<int>(m_bufferSize), 0);
    if (iResult == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK && m_mode == Mode::Nonblocking)
        {
            return TCPData{std::numeric_limits<std::size_t>::max(), NULL};
        }
        else
        {
			std::ostrstream msg;
			msg << "Call to receive failed, error = " << error << std::ends;
			closesocket(m_socket);
			throw std::runtime_error(msg.str());
        }
    }
    return TCPData{ static_cast<std::size_t>(iResult), m_bufferPtr };
}

bool TCPSocket::isDataAvailable()
{
    if (m_mode == Mode::Nonblocking)
    {
        FD_ZERO(&m_fdSet);
        FD_SET(m_socket, &m_fdSet);
        int iResult = select(0, &m_fdSet, NULL, NULL, &m_timeval);
        if (iResult == SOCKET_ERROR)
        {
            std::ostrstream msg;
            msg << "Select on socket in non-blocking mode failed, error = " << WSAGetLastError() << std::ends;
            closesocket(m_socket);
            throw std::runtime_error(msg.str());
        }
        return FD_ISSET(m_socket, &m_fdSet);
    }
    else
    {
        throw std::runtime_error("Function isDataAvailable is only valid in non-blocking mode");
    }
}

void TCPSocket::shutdown()
{
    int iResult = ::shutdown(m_socket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
		std::ostrstream msg;
		msg << "Call to shutdown failed, error = " << WSAGetLastError() << std::ends;
		closesocket(m_socket);
		throw std::runtime_error(msg.str());
    }
}

void TCPSocket::close()
{
    int iResult = closesocket(m_socket);
    if (iResult == SOCKET_ERROR)
    {
		std::ostrstream msg;
		msg << "Call to closesocket failed, error = " << WSAGetLastError() << std::ends;
		throw std::runtime_error(msg.str());
    }
    m_socket = INVALID_SOCKET;
}
