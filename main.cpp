#include "include/cpp/IO.h"

using namespace std;
using namespace boost;
using namespace com::myselia::cpp;


int main(int argc, char** argv)
{
	char val;

	Socket socket("127.0.0.1", 7000);

	boost::shared_ptr<InputStream> is=socket.getInputStream();
	boost::shared_ptr<OutputStream> os=socket.getOutputStream();

	while(true)
	{
		if((val=is->read())==-1)
			break;

		cout << val;

		os->write(val);
	}

	return 0;
}
