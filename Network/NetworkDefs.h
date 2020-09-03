#ifndef NETWORK_DEFS_H
#define NETWORK_DEFS_H

#include <winsock2.h>
#include <ws2tcpip.h>

enum class Mode
{
	Invalid = 0,
	Blocking = 1,
	Nonblocking = 2
};

#endif // NETWORK_DEFS_H
