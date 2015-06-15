#include <daemon/DaemonServer.h>

using namespace std;
using namespace boost;
using namespace com::myselia::daemon;

int main(int argc, char** argv)
{
	DaemonServer server(7000);

	boost::this_thread::sleep(boost::posix_time::milliseconds(10000));

	return 0;
}
