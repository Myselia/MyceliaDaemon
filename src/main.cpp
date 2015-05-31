#include <cppcommon/IO.h>

using namespace std;
using namespace boost;
using namespace com::myselia::cppcommon;

void connectionHandler(boost::shared_ptr<Socket> socket);

int main(int argc, char** argv)
{
	ServerSocket serverSocket(7000);
	boost::shared_ptr<Socket> socket;

	while(true)
	{
		socket=serverSocket.accept();

		boost::thread(boost::bind(&connectionHandler, socket));
	}

	return 0;
}

void connectionHandler(boost::shared_ptr<Socket> socket)
{
	char val;
	boost::shared_ptr<InputStream> is=socket->getInputStream();
	boost::shared_ptr<OutputStream> os=socket->getOutputStream();

	while(true)
	{
		if((val=is->read())==-1)
			break;

		cout << val;

		os->write(val);
	}
}
