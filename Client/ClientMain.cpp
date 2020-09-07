
#include <Network.h>
#include <iostream>
#include <thread>
#include <limits>

using namespace std::chrono_literals;

#ifdef max
#undef max
#endif

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
			socket = client->connect(std::string("MathTixWinDev"), 48000, Mode::Nonblocking);

			std::string msg("Send me some data");
			socket.send(msg.c_str(), msg.length());
			socket.shutdown();
			
			bool gotTheMessage = false;
			do {
				std::cout << "Waiting for server to send data" << std::endl;
				std::this_thread::sleep_for(1s);

				if (socket.isDataAvailable())
				{
					do {
						buf = socket.receive();
						if (buf.getLength() == std::numeric_limits<std::size_t>::max())
						{
							break;
						}
						else
						{
							std::string msg(buf.getData(), buf.getLength());
							std::cout << msg << std::endl;
						}
					} while (buf.getLength() > 0);
					gotTheMessage = true;
				}
			} while (!gotTheMessage);
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