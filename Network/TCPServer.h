#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <Platform.h>
#include <NetworkDefs.h>
#include <TCPSocket.h>

class TCPServer
{
	friend class Network;
protected:
	TCPServer(Mode mode = Mode::Blocking);

public:
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
	struct timeval m_timeval;
};

#endif // TCP_SERVER_H
