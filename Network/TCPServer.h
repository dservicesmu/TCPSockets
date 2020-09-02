#ifndef TCPSERVER_H
#define TCPSERVER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include "TCPSocket.h"

class TCPServer
{
public:
	TCPServer() {}
	~TCPServer() {}

	void bind(std::uint16_t port);
	void listen();
	TCPSocket accept();

protected:

private:
    SOCKET m_listenSocket = INVALID_SOCKET;
};

#endif // TCPSERVER_H
