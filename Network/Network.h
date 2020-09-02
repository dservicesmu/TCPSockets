#ifndef NETWORK_H
#define NETWORK_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <list>
#include <memory>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "TCPServer.h"


class Network
{
public:
	Network() { }
	~Network() { }

	void initialize();
	void uninitialize();

	std::shared_ptr<TCPServer>& createTCPServer();

protected:
private:
	std::list<std::shared_ptr<TCPServer>> m_tcpServers;
};

#endif // NETWORK_H
