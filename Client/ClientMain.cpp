
#include <Network.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	try
	{
		std::unique_ptr<Network> network(new Network());
		network->initialize();

		try
		{
			std::shared_ptr<TCPClient> client;
			TCPSocket socket;
			TCPData buf;

			client = network->createTCPClient();
			socket = client->connect(std::string("MathTixWinDev"), 48000);

			std::string msg("Send me some data");
			socket.send(msg.c_str(), msg.length());
			socket.shutdown();

			do {
				buf = socket.receive();
				std::string msg(buf.getData(), buf.getLength());
				std::cout << msg << std::endl;
			} while (buf.getLength() > 0);
		}
		catch(std::runtime_error& rt)
		{
			std::cerr << "Client error = " << rt.what() << std::endl;
		}
		catch (std::exception e)
		{
			std::cerr << "C++ exception = " << e.what() << std::endl;
		}

		network->uninitialize();
	}
	catch (std::runtime_error& rt)
	{
		std::cerr << "Client error = " << rt.what() << std::endl;
	}
	catch (std::exception e)
	{
		std::cerr << "C++ exception = " << e.what() << std::endl;
	}
	return 0;
}