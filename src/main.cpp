#include <daemon/DaemonServer.h>
#include <daemon/Config.h>

using namespace std;
using namespace boost;
using namespace com::myselia::cppcommon;
using namespace com::myselia::daemon;
using namespace com::myselia::common::communication::units;

void executeDaemon(Configuration& config);
void executeTestClient();
void clientReceiveResult(boost::shared_ptr<Transmission> transmission, bool& operationDone);
void sendRequestToDaemon(boost::shared_ptr<TransmissionService> ts,
		string daemonHost, uint daemonPort, string daemonComponentId,
		string localHost, uint localPort, Opcode localOpcode, string filename);
void sendFileThread(uint localPort, string filename);
void sendFileToDaemon(boost::shared_ptr<TransmissionService> ts,
		string daemonHost, uint daemonPort, string daemonComponentId,
		Opcode localOpcode, string filename);
void clientSendExecuteToDaemon(boost::shared_ptr<TransmissionService> ts,
		string daemonComponentId, Opcode localOpcode, string command, string username, string password);

int main(int argc, char** argv)
{
	Configuration config("config.json");

	if(argc==2)
		executeTestClient();
	else
		executeDaemon(config);

	return 0;
}

void executeDaemon(Configuration& config)
{
	DaemonServer server(config.getPort(), config.getUsername(), config.getPassword());

	while(true)
		boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
}

void executeTestClient()
{
	//Daemon info
	string daemonHost="127.0.0.1";
	uint daemonPort=7000;
	string daemonComponentId="42";
	string username="user";
	string password="pass";
	//Local info
	string filename="sandbox.jar";
	string command="java -jar "+filename;
	Opcode receiveAnswerOpcode(ComponentType::STEM, ActionType::RUNTIME, "receiveAnser");
	bool operationDone;

	//Connect to Daemon Server
	boost::shared_ptr<Socket> socket(new Socket(daemonHost, daemonPort));
	boost::shared_ptr<BasicTransmissionService> bts(new BasicTransmissionService());
	bts->addChannel(socket);

	//Setup handlers
	bts->addListener(receiveAnswerOpcode, boost::bind(&clientReceiveResult, _1, boost::ref(operationDone)));

	operationDone=false;
	cout << "-------------Sending file to Daemon-------------" << endl;
	sendFileToDaemon(bts, daemonHost, daemonPort, daemonComponentId, receiveAnswerOpcode, filename);

	while(!operationDone)
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

	cout << "-------------Executing file-------------" << endl;
	operationDone=false;
	clientSendExecuteToDaemon(bts, daemonComponentId, receiveAnswerOpcode, command, username, password);

	while(!operationDone)
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

	cout << "-------------Done-------------" << endl;
}

void sendFileToDaemon(boost::shared_ptr<TransmissionService> ts,
		string daemonHost, uint daemonPort, string daemonComponentId,
		Opcode localOpcode, string filename)
{
	//Local info
	string localHost="127.0.0.1";
	uint localPort=7007;

	//Get ready to send file
	boost::thread(boost::bind(&sendFileThread, localPort, filename));

	//Send request to Daemon
	sendRequestToDaemon(ts, daemonHost, daemonPort, daemonComponentId, localHost, localPort, localOpcode, filename);

	cout << "Waiting for response..." << endl;
}

void sendFileThread(uint localPort, string filename)
{
	ServerSocket ss(localPort);

	cout << "Waiting for Daemon to connect..." << endl;
	boost::shared_ptr<Socket> s=ss.accept();
	boost::shared_ptr<OutputStream> os=s->getOutputStream();

	cout << "Sending file..." << endl;

	std::ifstream ifs(filename, std::ios::binary);

	if(!ifs.good())
		throw GenericException("Unable to read file: '"+filename+"'");

	char buf[1024];

	while(ifs.good()) {
		ifs.read(buf, sizeof(buf));

		ByteBuffer bytebuf(buf, ifs.gcount());

		os->write(bytebuf);
	}

	if(!ifs.eof())
		throw GenericException("Error while reading file: '"+filename+"'");

	ifs.close();

	cout << "File sent." << endl;
}

void sendRequestToDaemon(boost::shared_ptr<TransmissionService> ts,
		string daemonHost, uint daemonPort, string daemonComponentId,
		string localHost, uint localPort, Opcode localOpcode,
		string filename)
{
	Sha1Hash hash=GenericUtil::getSha1Hash(filename);

	Destination to(daemonComponentId, Opcode(ComponentType::DAEMON, ActionType::RUNTIME, "receiveFile"));

	//Send receiveSandbox request.
	boost::shared_ptr<Transmission> transmission(new Transmission(to));
	transmission->setFrom(Destination("", localOpcode).toString()); //componentId will get replaced by TransmissionService.
	transmission->addStringAtom(filename);
	transmission->addStringAtom(GenericUtil::getHexString(hash, hash.getSizeInBytes()));
	transmission->addStringAtom(localHost);
	transmission->addStringAtom(std::to_string(localPort));

	cout << "Sending request..." << endl;
	ts->send(transmission);
	cout << "Request sent." << endl;
}

void clientReceiveResult(boost::shared_ptr<Transmission> transmission, bool& operationDone)
{
	cout << "Received answer." << endl;

	string output=transmission->getAtoms()[0]->get_value();

	cout << "Answer: " << output << endl;

	operationDone=true;
}

void clientSendExecuteToDaemon(boost::shared_ptr<TransmissionService> ts,
		string daemonComponentId, Opcode localOpcode, string command, string username, string password)
{
	Destination to(daemonComponentId, Opcode(ComponentType::DAEMON, ActionType::RUNTIME, "executeCommand"));

	//Send receiveSandbox request.
	boost::shared_ptr<Transmission> transmission(new Transmission(to));
	transmission->setFrom(Destination("", localOpcode).toString()); //componentId will get replaced by TransmissionService.
	transmission->addStringAtom(username);
	transmission->addStringAtom(password);
	transmission->addStringAtom(command);

	cout << "Sending request..." << endl;
	ts->send(transmission);
	cout << "Request sent." << endl;
}
