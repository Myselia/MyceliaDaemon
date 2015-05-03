#include "../include/MyceliaDaemon.h"

using namespace std;
using namespace com::myselia;
using namespace com::myselia::cpp;

MyceliaDaemon::MyceliaDaemon()
{
	running=false;
	serverThread=NULL;
	serverPort=0;
}

void MyceliaDaemon::start()
{
	//Use any available port
	start(0);
}

void MyceliaDaemon::start(int port)
{
	if(running)
		throw IllegalStateException("Daemon already started");

	running=true;

	serverThread=new boost::thread(boost::bind(&MyceliaDaemon::serverHandlerThread, this, port));
}

void MyceliaDaemon::stop()
{
	if(!running)
		throw IllegalStateException("Daemon not running");

	io_service.stop();
	serverThread->join();
	delete serverThread;
	serverThread=NULL;

	connectionThreads.interrupt_all();
	connectionThreads.join_all();

	running=false;
}

void MyceliaDaemon::serverHandlerThread(int port)
{
	//port might be 0 meaning any port

	boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));

	serverPort=acceptor.local_endpoint().port();
	cout << "Running on port " << serverPort << endl;

	startAccept(acceptor);

	io_service.run();
}

void MyceliaDaemon::startAccept(boost::asio::ip::tcp::acceptor& acceptor)
{
	  boost::shared_ptr<boost::asio::ip::tcp::socket> socket(new boost::asio::ip::tcp::socket(io_service));

	  acceptor.async_accept(*socket, boost::bind(&MyceliaDaemon::connectionHandlerThread, this, boost::asio::placeholders::error, socket, boost::ref(acceptor)));
}

void MyceliaDaemon::connectionHandlerThread
(
		const boost::system::error_code& error,
        boost::shared_ptr<boost::asio::ip::tcp::socket> socket,
        boost::asio::ip::tcp::acceptor& acceptor
)
{
	  if (error)
	  {
		  cout << "Error accepting connection: " << error.message() << endl;
		  return;
	  }

	  startAccept(acceptor);

	  std::string message = "test 42 NML";

	 boost::system::error_code write_error;
	 boost::asio::write(*socket, boost::asio::buffer(message), write_error);

	 cout << "Error writing: " << write_error.message() << endl;
}
