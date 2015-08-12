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
	DaemonOperations(boost::shared_ptr<TransmissionService> ts, string username, string password): ts(ts), username(username), password(password)
	{
		hookTasks();
	}

	private:
	string username;
    string password;
	boost::shared_ptr<TransmissionService> ts;

	void hookTasks()
	{
		Opcode opcode1(ComponentType::DAEMON, ActionType::RUNTIME, "executeCommand");
		ts->addListener(opcode1, boost::bind(&DaemonOperations::executeCommand, this, opcode1, _1));

		Opcode opcode2(ComponentType::DAEMON, ActionType::RUNTIME, "receiveFile");
		ts->addListener(opcode2, boost::bind(&DaemonOperations::receiveFile, this, opcode2, _1));
	}

	void receiveFile(Opcode opcode, boost::shared_ptr<Transmission> argsTrans)
	{
		string filename=argsTrans->getAtoms()[0]->get_value();
		string sha1hash=argsTrans->getAtoms()[1]->get_value();
		string host=argsTrans->getAtoms()[2]->get_value();
		uint port=std::stoi(argsTrans->getAtoms()[3]->get_value());

		cout << "Receiving File '" << filename << "' with SHA1 hash " << sha1hash << " from " << host << ":" << port << endl;

		cout << "Connecting..." << endl;
		//Connecting
		boost::shared_ptr<Socket> s(new Socket(host, port));
		boost::shared_ptr<InputStream> is=s->getInputStream();

		cout << "Receiving file..." << endl;
		//Receiving file
		std::ofstream ofs(filename, std::ios::binary|std::ios::trunc);

		if(!ofs.good())
			throw GenericException("Unable to open file: '"+filename+"' for output.");

		size_t bytesRead=0;
		size_t count=0;
		ByteBuffer buf(1024*1024);

		while(ofs.good()&&(bytesRead=is->read(buf))!=-1)
		{
			ofs.write(buf.getAsCstring(), bytesRead);
			count+=bytesRead;
			cout << "Read " << count << " bytes." << endl;
		}

		if(!ofs.good())
			throw GenericException("Error while writing to file: '"+filename+"'");

		ofs.close();

		cout << "File received. Checking file..." << endl;

		Sha1Hash hash=GenericUtil::getSha1Hash(filename);

		string answer;

		if(sha1hash==GenericUtil::getHexString(hash, hash.getSizeInBytes()))
		{
			cout << "File passed check." << endl;
			answer="OK";
		}
		else
		{
			cout << "File failed check." << endl;
			answer="NOK";
		}

		//setting permissions on file
		boost::filesystem::permissions(filename, boost::filesystem::add_perms|boost::filesystem::owner_all);

		//Send answer.
		boost::shared_ptr<Transmission> retTrans=boost::shared_ptr<Transmission>(new Transmission(argsTrans->getFrom()));
		retTrans->setFrom(Destination("", opcode).toString()); //componentId will get replaced by TransmissionService.
		retTrans->addStringAtom(answer);

		cout << "Sending answer..." << endl;
		ts->send(retTrans);
		cout << "Answer sent." << endl;
	}

	void executeCommand(Opcode opcode, boost::shared_ptr<Transmission> argsTrans)
	{
		string username=argsTrans->getAtoms()[0]->get_value();
		string password=argsTrans->getAtoms()[1]->get_value();
		string command=argsTrans->getAtoms()[2]->get_value();

		if(this->username!=username || this->password!=password)
		{
			cout << "Warning: wrong username and password, ignoring executeCommand task." << endl;
			return;
		}

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
	DaemonServer(uint port, string username, string password): port(port)
	{
		bts=boost::shared_ptr<BasicTransmissionService>(new BasicTransmissionService(getComponentId()));

		daemonTasks=boost::shared_ptr<DaemonOperations>(new DaemonOperations(bts, username, password));

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
	string username;
	string password;
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
		//TODO change algorithm for something more apropriate.

		return "42";
	}
};

}
}
}

#endif /* INCLUDE_DAEMON_DAEMONSERVER_H_ */
