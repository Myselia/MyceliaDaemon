#include "include/cpp/IO.h"

using namespace std;
using namespace boost;
using namespace com::myselia::cpp;


int main(int argc, char** argv)
{
	char cbuf[5];
	asio::mutable_buffers_1 buffer(cbuf, sizeof(cbuf));

	cout << "*1*" << endl;

	Socket socket("127.0.0.1", 7000);

	cout << "*2*" << endl;

	boost::shared_ptr<InputStream> is=socket.getInputStream();

	cout << "*3*" << endl;

	while(true)
	{
		cout << "*4*" << endl;
		is->read(buffer);
		cout << "*5*" << endl;

		cout << (char*)asio::buffer_cast<char*>(buffer);

		cout << "*6*" << endl;
	}

	return 0;
}
