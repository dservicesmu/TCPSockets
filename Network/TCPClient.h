#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <Platform.h>
#include <NetworkDefs.h>
#include <TCPSocket.h>


class TCPClient
{
	friend class Network;
protected:
	TCPClient() {}

public:
	~TCPClient() {}

	TCPSocket connect(
		std::string const& host,
		std::uint16_t port
		);

protected:

private:

};

#endif // TCP_CLIENT_H