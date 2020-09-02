#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <Platform.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class TCPData
{
public:
	TCPData() : m_length(0), m_dataPtr(NULL) { }
	TCPData(
		std::size_t length,
		char* dataPtr
		) : m_length(length)
		  , m_dataPtr(dataPtr)
	{
	}

	~TCPData()
	{
		m_length = 0;
		m_dataPtr = NULL;
	}

	std::size_t getLength() { return m_length;  }
	char* getData() { return m_dataPtr;  }
	
private:
	std::size_t m_length;
	char* m_dataPtr;
};

class TCPSocket
{
public:
	TCPSocket();
	TCPSocket(
		SOCKET socket,
		std::size_t bufferSize = 256
		);
	TCPSocket(const TCPSocket& src);

	~TCPSocket();

	TCPSocket& operator=(const TCPSocket& src);

	std::string getAddress();
	void send(
		char const* dataPtr,
		std::size_t length
		);
	TCPData receive();

protected:
	SOCKET getSocket() { return m_socket;  }
	void setSocket(SOCKET socket) { m_socket = socket;  }

private:
	std::size_t m_bufferSize;
	std::size_t m_receiveSize;
	char* m_bufferPtr;
	SOCKET m_socket;
};

#endif  // TCP_SOCKET_H
