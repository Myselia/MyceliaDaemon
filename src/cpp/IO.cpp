#include "../../include/cpp/IO.h"

using namespace std;
using namespace com::myselia::cpp;

//IoService
IoService::IoService()
{
	//Tell the io_service not to stop running until <code>work</code> is destroyed.
	work=new asio::io_service::work(io_service);

	//Start multiple threads as workers to handle io_service async operations.
	for(int i=0; i<NUMBER_OF_WORKER; i++)
		serviceThreads.create_thread(bind(&IoService::serviceHandlerThread, this));
}

IoService::~IoService()
{
	//Tell io_service to stop once all pending operations are done.
	delete work;
	io_service.stop();

	//Wait for all pending operations to be done.
	serviceThreads.join_all();
}

asio::io_service& IoService::getBoostIoService()
{
	return io_service;
}

void IoService::serviceHandlerThread()
{
	io_service.run();
}

//InputStream
InputStream::InputStream(Socket* socket): socket(socket)
{
	inProgress=false;
}

size_t InputStream::read(asio::mutable_buffers_1& buffer)
{
	//Start async read
	inProgress=true;

	cout << "*4-1*" << endl;

	async_read(*(socket->getAsioSocket()), buffer, bind(&InputStream::readDone, this, asio::placeholders::error));

	cout << "*4-2*" << endl;

	//Wait till async read is done.
	while(inProgress)
		boost::this_thread::sleep(boost::posix_time::milliseconds(WAIT_TIME));

	cout << "*4-3*" << endl;

	//Look for errors while reading
	if(errorReading)
	{
		if(errorReading==asio::error::eof)
			return -1;
		else
			throw new IOException(errorReading);
	}

	cout << "*4-4*" << endl;

	return asio::buffer_size(buffer);
}

void InputStream::readDone(const system::error_code& error)
{
	cout << "*4-1-1*" << endl;
	errorReading=error;
	inProgress=false;

	cout << "*4-1-2*" << endl;
}

//OutputStream
OutputStream::OutputStream(Socket* socket): socket(socket)
{
	inProgress=false;
}

void OutputStream::write(asio::mutable_buffers_1& buffer)
{
	//Start async write
	inProgress=true;
	async_write(*socket->getAsioSocket(), buffer, bind(&OutputStream::writeDone, this, asio::placeholders::error));

	//Wait till async write is done.
	while(inProgress)
		boost::this_thread::sleep(boost::posix_time::milliseconds(WAIT_TIME));

	//Look for errors while writing
	if(errorWriting)
		throw new IOException(errorWriting);
}

void OutputStream::writeDone(const system::error_code& error)
{
	errorWriting=error;
	inProgress=false;
}

//Socket
Socket::Socket(const boost::shared_ptr<asio_socket>& socket): socket(socket)
{
	connectPending=false;
}

Socket::Socket(string host, int port): socket(boost::shared_ptr<asio_socket>(new asio_socket(IoService::getInstance().getBoostIoService())))
{
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(host), port);

	connectPending=true;
	socket->async_connect(ep, bind(&Socket::connectDone, this, asio::placeholders::error));

	//Wait till async connect is done.
	while(connectPending)
		boost::this_thread::sleep(boost::posix_time::milliseconds(WAIT_TIME));

	//Look for errors while connecting
	if(errorConnecting)
		throw new IOException(errorConnecting);
}

void Socket::connectDone(const system::error_code& error)
{
	errorConnecting=error;
	connectPending=false;
}

boost::shared_ptr<asio_socket> Socket::getAsioSocket()
{
	return socket;
}

boost::shared_ptr<InputStream> Socket::getInputStream()
{
	return boost::shared_ptr<InputStream>(new InputStream(this));
}

boost::shared_ptr<OutputStream> Socket::getOutputStream()
{
	return boost::shared_ptr<OutputStream>(new OutputStream(this));
}
