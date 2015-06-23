#include <daemon/DaemonServer.h>

using namespace std;
using namespace boost;
using namespace com::myselia::daemon;
using namespace com::myselia::common::communication::units;

void executeDaemon();
void executeTestClient();

int main(int argc, char** argv)
{
	executeDaemon();

	return 0;
}

void executeDaemon()
{
	DaemonServer server(7000);

	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
}

void executeTestClient()
{
	boost::shared_ptr<Socket> socket(new Socket("127.0.0.1", 7000));
	BasicTransmissionService bts("clientId");
	bts.addChannel(socket);

	Destination destination("42", Opcode(ComponentType::DAEMON, ActionType::RUNTIME, "executeCommand"));


	boost::shared_ptr<Transmission> transmission(new Transmission(destination.toString()));
	transmission->addStringAtom("uname -a");

	cout << "Sending command..." << endl;
	bts.send(destination, transmission);
	cout << "Sent command." << endl;
}
