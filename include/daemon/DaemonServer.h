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

class DaemonTasks
{
	public:
	DaemonTasks(boost::shared_ptr<TransmissionService> ts): ts(ts)
	{
		hookTasks();
	}

	private:
	boost::shared_ptr<TransmissionService> ts;

	void hookTasks()
	{
		ts->addListener(Opcode(ComponentType::DAEMON, ActionType::RUNTIME, "executeCommand"), boost::bind(&DaemonTasks::executeCommand, this, _1));
	}

	void executeCommand(boost::shared_ptr<Transmission> transmission)
	{
		string command=transmission->get_atoms()[0]->get_value();

		cout << "Executing command: " << command << endl;
	}
};

class DaemonServer
{
	public:
	DaemonServer(uint port): port(port)
	{
		bts=boost::shared_ptr<BasicTransmissionService>(new BasicTransmissionService(getComponentId()));

		daemonTasks=boost::shared_ptr<DaemonTasks>(new DaemonTasks(bts));

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
	boost::shared_ptr<DaemonTasks> daemonTasks;

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
		//TODO more appropriate algorithm.
		return "42";
	}
};

}
}
}

#endif /* INCLUDE_DAEMON_DAEMONSERVER_H_ */
