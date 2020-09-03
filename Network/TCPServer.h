#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <Platform.h>
#include <NetworkDefs.h>
#include <TCPSocket.h>

class TCPServer
{
public:
	TCPServer(Mode mode = Mode::Nonblocking);
	~TCPServer();

	Mode getMode() { return m_mode; }

	void bind(std::uint16_t port);
	void listen();
	void stopListen();
	TCPSocket accept();


protected:

private:
	Mode m_mode;
	FD_SET m_listenSet;
    SOCKET m_listenSocket;
};

#endif // TCP_SERVER_H
