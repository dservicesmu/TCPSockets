#ifndef NETWORK_H
#define NETWORK_H

#include <Platform.h>
#include <NetworkDefs.h>
#include <TCPServer.h>
#include <TCPClient.h>

#include <list>
#include <memory>

class Network
{
public:
	Network() { }
	~Network() { }

	void initialize();
	void uninitialize();

	std::shared_ptr<TCPServer>& createTCPServer();
	std::shared_ptr<TCPClient>& createTCPClient();

protected:

private:
	std::list<std::shared_ptr<TCPServer>> m_tcpServers;
	std::list<std::shared_ptr<TCPClient>> m_tcpClients;
};

#endif // NETWORK_H
