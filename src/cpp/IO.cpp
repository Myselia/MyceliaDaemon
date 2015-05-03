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
	//Do nothing
}

int InputStream::read(asio::mutable_buffers_1& buffer)
{
	try
	{
		future<size_t> future=async_read(*(socket->getAsioSocket()), buffer, boost::asio::use_future);

		size_t bytesRead=future.get();
		return bytesRead;
	}
	catch(system::system_error& error)
	{
		if(error.code()==asio::error::eof)
			return -1;
		else
			throw IOException(error.code());
	}
}

int InputStream::read()
{
	char cbuf[1];
	asio::mutable_buffers_1 buffer(cbuf, sizeof(cbuf));

	if(read(buffer)==-1)
		return -1;

	uchar val=cbuf[0];

	return val;
}

//OutputStream
OutputStream::OutputStream(Socket* socket): socket(socket)
{
	//Do nothing
}

void OutputStream::write(asio::mutable_buffers_1& buffer)
{
	try
	{
		future<size_t> future=async_write(*socket->getAsioSocket(), buffer, boost::asio::use_future);

		//Wait till the operation is done
		future.get();
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
}

void OutputStream::write(uchar val)
{
	char cbuf[1];
	cbuf[0]=val;

	asio::mutable_buffers_1 buffer(cbuf, sizeof(cbuf));

	write(buffer);
}

//Socket
Socket::Socket(boost::shared_ptr<asio_socket> socket): socket(socket)
{
	//Do nothing
}

Socket::Socket(string host, int port): socket(boost::shared_ptr<asio_socket>(new asio_socket(IoService::getInstance().getBoostIoService())))
{
	try
	{
		asio_endpoint ep(asio::ip::address::from_string(host), port);
		future<void> future=socket->async_connect(ep, boost::asio::use_future);

		//Wait till the operation is done
		future.get();
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
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

//ServerSocket
ServerSocket::ServerSocket(int port)
{
	try
	{
		acceptor=boost::shared_ptr<asio_acceptor>(new asio_acceptor(IoService::getInstance().getBoostIoService(), asio_endpoint(boost::asio::ip::tcp::v4(), port)));
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
}

boost::shared_ptr<Socket> ServerSocket::accept()
{
	try
	{
		boost::shared_ptr<asio_socket> socket(new asio_socket(IoService::getInstance().getBoostIoService()));

		future<void> future=acceptor->async_accept(*socket, boost::asio::use_future);

		//Wait till the operation is done
		future.get();

		return boost::shared_ptr<Socket>(new Socket(socket));
	}
	catch(system::system_error& error)
	{
		throw IOException(error.code());
	}
}

