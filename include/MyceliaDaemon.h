
#ifndef INCLUDE_MYCELIADAEMON_H_
#define INCLUDE_MYCELIADAEMON_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "cpp/Json.h"

namespace com
{
namespace myselia
{

class MyceliaDaemon
{
	public:
	MyceliaDaemon();

	void start();
	void start(int port);
	void stop();

	private:
	bool running;
	ushort serverPort;
	boost::asio::io_service io_service;
	boost::thread* serverThread;
	boost::thread_group connectionThreads;

	void serverHandlerThread(int port);
	void connectionHandlerThread
	(
			const boost::system::error_code& error,
	        boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
	        boost::asio::ip::tcp::acceptor& acceptor
	);

	void startAccept(boost::asio::ip::tcp::acceptor& acceptor);
};

}
}

#endif /* INCLUDE_MYCELIADAEMON_H_ */
