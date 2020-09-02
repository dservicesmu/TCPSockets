#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <Platform.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <TCPSocket.h>

class TCPClient
{
public:
	TCPClient() {}
	~TCPClient() {}

	TCPSocket connect(
		std::string const& host,
		std::uint16_t port
		);

protected:

private:

};

#endif // TCP_CLIENT_H