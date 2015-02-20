#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

typedef struct {
	std::string ip;
	std::string port;
	int nodeID;
	int stemID;
	bool ready;
} stemConnection;

typedef struct {
	std::string ip;
	std::string port;
	std::string invokePath;
	bool ready;
} sandboxConnection;

namespace MyceliaCom {

/*
 * Interfaces
 */
class ICommunicator {
public:
	virtual ~ICommunicator() {}
	virtual int establishConnection() = 0;
	virtual int send();
	virtual int recv();
private:
	virtual void populateConnectionInfo() = 0;
	std::string in_;
	std::string out_;
	boost::asio::buffer in(in_);
	boost::asio::buffer out(out_);
};

class ISeekImpl {
public:
	virtual ~ISeekImpl() {}
	//Pass it a stemConnection reference to populate with stem information once seeked
	virtual int seekStem(stemConnection & comm) = 0;
};

/*
 * Concretes
 */
class MyceliaCommunicator: public ICommunicator {
public:
	int establishConnection();
private:
	populateConnectionInfo();
};

/*
 * Concretes for MyceliaCommunicator
 */
class TCP_Server {

};

class TCP_Connection: public boost::enable_shared_from_this<tcp_connection> {

};

class UDP_Client {

};

/*
 * ----------------------------------
 */

class BasicSeek: public ISeekImpl {
public:
	int seekStem(UDP_Client & seekListener, stemConnection & comm) {

	}
};

}

