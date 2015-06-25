#include <daemon/DaemonServer.h>

using namespace std;
using namespace boost;
using namespace com::myselia::daemon;
using namespace com::myselia::common::communication::units;

void executeDaemon();
void executeTestClient();
void clientReceiveResult(boost::shared_ptr<Transmission> transmission);

int main(int argc, char** argv)
{
	if(argc<2)
	{
		cout << "Error: needs exactly 1 argument." << endl;
		return -1;
	}

	if(strcmp(argv[1], "server")==0)
		executeDaemon();
	else
		executeTestClient();

	return 0;
}

void executeDaemon()
{
	DaemonServer server(7000);

	while(true)
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
}

void executeTestClient()
{
	Destination to("42", Opcode(ComponentType::DAEMON, ActionType::RUNTIME, "executeCommand"));
	Opcode fromOpcode(ComponentType::STEM, ActionType::RUNTIME, "sendCommandToDaemon");

	//Connect to Daemon Server
	boost::shared_ptr<Socket> socket(new Socket("127.0.0.1", 7000));
	BasicTransmissionService bts("clientId");
	bts.addChannel(socket);

	//Setup handlers
	bts.addListener(fromOpcode, boost::bind(&clientReceiveResult, _1));

	//Send exec request.
	boost::shared_ptr<Transmission> transmission(new Transmission(to));
	transmission->setFrom(Destination("", fromOpcode).toString()); //componentId will get replaced by TransmissionService.
	transmission->addStringAtom("uname -a");

	cout << "Sending command..." << endl;
	bts.send(transmission);
	cout << "Sent command. Waiting for response..." << endl;

	while(true)
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
}

void clientReceiveResult(boost::shared_ptr<Transmission> transmission)
{
	cout << "Received answer." << endl;

	string commandOutput=transmission->getAtoms()[0]->get_value();

	cout << "Answer: " << commandOutput << endl;
}
