#ifndef INCLUDE_CPP_IO_H_
#define INCLUDE_CPP_IO_H_

/* IO.h : classes for Network IO.
 *
 * The following classes are wrapper around boost ASIO to expose a similar interface as the java library.
 * This is to allow easier translation of the MyseliaCommon to cpp.
 */

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Json.h"

using namespace boost;

typedef boost::asio::ip::tcp::socket asio_socket;

namespace com
{
namespace myselia
{
namespace cpp
{

class IOException: public GenericException
{
	public:
	IOException()
	{
		//Do nothing
	}

	IOException(string message): GenericException(message)
	{
		//Do nothing
	}

	IOException(exception cause): GenericException(cause)
	{
		//Do nothing
	}

	IOException(system::error_code error): GenericException(error.message())
	{
		//Do nothing
	}
};

class IoService
{
	public:
	static IoService& getInstance()
	{
		static IoService instance;

		return instance;
	}

	asio::io_service& getBoostIoService();

	private:
	int const NUMBER_OF_WORKER=2;
	asio::io_service io_service;
	asio::io_service::work* work;
	thread_group serviceThreads;

	IoService();
	~IoService();

	IoService(IoService const&); //Dont implement (for singleton)
	void operator=(IoService const&); //Dont implement (for singleton)

	void serviceHandlerThread();
};

class Socket;

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * WARNING: This class is not thread safe.
 */
class InputStream
{
	public:
	InputStream(Socket* socket);

	/**
	 * Read from this stream, this will attempt to fill the buffer with data.
	 * This returns the number of bytes read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * throws IOException
	 */
	size_t read(asio::mutable_buffers_1& buffer);

	/**
	 * Read one byte from this stream.
	 * This returns the byte read or -1 if the end of stream is reached (if the connection is closed).
	 *
	 * The byte read is returned as an unsigned char for compatibility with Java API.
	 *
	 * throws IOException
	 */
	int read();

	private:
	/**
	 * Time to atomically wait in milliseconds for an asynchronous operations to finish.
	 */
	int const WAIT_TIME=30;
	Socket* socket;
	bool inProgress;
	system::error_code errorReading;

	void readDone(const system::error_code& error);
};

/**
 * This class imitates the java class of the same name but uses boost::asio underneath.
 * WARNING: This class is not thread safe.
 */
class OutputStream
{
	public:
	OutputStream(Socket* socket);

	/**
	 * Write then entire buffer to this output stream.
	 *
	 * throws IOException
	 */
	void write(asio::mutable_buffers_1& buffer);

	/**
	 * Write one byte to this output stream.
	 *
	 * throws IOException
	 */
	void write(uchar val);

	private:
	/**
	 * Time to atomically wait in milliseconds for an asynchronous operations to finish.
	 */
	int const WAIT_TIME=30;
	Socket* socket;
	bool inProgress;
	system::error_code errorWriting;

	void writeDone(const system::error_code& error);
};

class Socket
{
	public:
	Socket(boost::shared_ptr<asio_socket> socket);
	Socket(const string host, int port);

	boost::shared_ptr<asio_socket> getAsioSocket();
	boost::shared_ptr<InputStream> getInputStream();
	boost::shared_ptr<OutputStream> getOutputStream();

	private:
	/**
	 * Time to atomically wait in milliseconds for an asynchronous operations to finish.
	 */
	int const WAIT_TIME=30;
	boost::shared_ptr<asio_socket> socket;
	bool connectPending;
	system::error_code errorConnecting;

	void connectDone(const system::error_code& error);
};

}
}
}

#endif /* INCLUDE_CPP_IO_H_ */
