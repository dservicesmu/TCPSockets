#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <Platform.h>
#include <NetworkDefs.h>
#include <TCPSocket.h>

class TCPServer
{
public:
	TCPServer() {}
	~TCPServer() {}

	Mode getMode();
	void setMode(Mode mode);

	void bind(std::uint16_t port);
	void listen();
	TCPSocket accept();

protected:

private:
    SOCKET m_listenSocket = INVALID_SOCKET;
};

#endif // TCP_SERVER_H
