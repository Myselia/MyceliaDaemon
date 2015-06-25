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

class DaemonOperations
{
	public:
	DaemonOperations(boost::shared_ptr<TransmissionService> ts): ts(ts)
	{
		hookTasks();
	}

	private:
	boost::shared_ptr<TransmissionService> ts;

	void hookTasks()
	{
		Opcode opcode1(ComponentType::DAEMON, ActionType::RUNTIME, "executeCommand");
		ts->addListener(opcode1, boost::bind(&DaemonOperations::executeCommand, this, opcode1, _1));
	}

	void executeCommand(Opcode opcode, boost::shared_ptr<Transmission> argsTrans)
	{
		string command=argsTrans->getAtoms()[0]->get_value();

		cout << "Execute command: " << command << endl;

		string output=GenericUtil::executeCommand(command);

		cout << "Output of command: " << output << endl;

		boost::shared_ptr<Transmission> retTrans=boost::shared_ptr<Transmission>(new Transmission(argsTrans->getFrom()));
		retTrans->setFrom(Destination("", opcode).toString()); //componentId will get replaced by TransmissionService.
		retTrans->addStringAtom(output);

		ts->send(retTrans);
	}
};

class DaemonServer
{
	public:
	DaemonServer(uint port): port(port)
	{
		bts=boost::shared_ptr<BasicTransmissionService>(new BasicTransmissionService(getComponentId()));

		daemonTasks=boost::shared_ptr<DaemonOperations>(new DaemonOperations(bts));

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
	boost::shared_ptr<DaemonOperations> daemonTasks;

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
