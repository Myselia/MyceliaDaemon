#ifndef INCLUDE_DAEMON_DAEMONSERVER_H_
#define INCLUDE_DAEMON_DAEMONSERVER_H_

#include <cppcommon/TransmissionService.h>

using namespace std;
using namespace boost;
using namespace com::myselia::cppcommon;

namespace com
{
namespace myselia
{
namespace daemon
{

class DaemonServer
{
	public:
	DaemonServer(uint port): port(port)
	{
		bts=boost::shared_ptr<BasicTransmissionService>(new BasicTransmissionService(getComponentId()));

		//Start server thread
		serverThread=boost::thread(bind(&DaemonServer::handleServer, this));
	}

	~DaemonServer()
	{
		//Stop server thread
		serverThread.interrupt();
		serverThread.join();
	}

	private:
	uint port;
	boost::shared_ptr<BasicTransmissionService> bts;
	boost::thread serverThread;

	void handleServer()
	{
		ServerSocket serverSocket(port);
		boost::shared_ptr<Socket> socket;

		while(true)
		{
			try
			{
				socket=serverSocket.accept();
			}
			catch(boost::thread_interrupted& e)
			{
				return;
			}

			bts->addChannel(socket);
		}
	}

	string getComponentId()
	{
		return "42";
	}
};

}
}
}

#endif /* INCLUDE_DAEMON_DAEMONSERVER_H_ */
