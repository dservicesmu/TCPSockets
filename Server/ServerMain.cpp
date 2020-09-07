
#include <Network.h>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try
	{
		std::unique_ptr<Network> network(new Network());
		network->initialize();

		try
		{
			std::shared_ptr<TCPServer> server;
			TCPSocket socket;
			std::uint16_t portNumber = 48000;

			server = network->createTCPServer(Mode::Nonblocking);
			server->bind(portNumber);
			server->listen();
			std::cout << "Server listening or port " << portNumber << std::endl;

			// Non-blocking update for listen socket
			/********************************************************************************/
			do {
				socket = server->accept();
				std::cout << "Waiting for client connection" << std::endl;
				std::this_thread::sleep_for(1s);
			} while (!socket.isValid());
			std::cout << "Client connected from " << socket.getAddress() << std::endl;
			/********************************************************************************/

			// Non-blocking update for socket to client.
			/********************************************************************************/
			TCPData buf;
			do {
				buf = socket.receive();
				std::string msg(buf.getData(), buf.getLength());
				if (buf.getLength() > 0)
				{
					std::cout << "Received message from client: " << msg << std::endl;
					std::string data("Here is your data");
					socket.send(&data[0], data.length());
				}
			} while (buf.getLength() > 0);
			/********************************************************************************/

			std::cout << "Client disconnecting..." << std::endl;
			server->stopListen();
			socket.shutdown();
			socket.close();
		}
		catch(std::runtime_error& rt)
		{
			std::cerr << "Server error = " << rt.what() << std::endl;
		}
		catch (std::exception e)
		{
			std::cerr << "C++ exception = " << e.what() << std::endl;
		}

		network->uninitialize();
	}
	catch (std::runtime_error& rt)
	{
		std::cerr << "Server error = " << rt.what() << std::endl;
	}
	catch (std::exception e)
	{
		std::cerr << "C++ exception = " << e.what() << std::endl;
	}
	return 0;
}