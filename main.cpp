#include "include/MyceliaDaemon.h"

using namespace std;
using namespace com::myselia;


int main(int argc, char** argv)
{
	MyceliaDaemon deamon;

	cout << "start" << endl;
	deamon.start();

	cout << "sleep" << endl;
	sleep(10);

	cout << "stop" << endl;
	deamon.stop();

	cout << "stopped" << endl;

	return 0;
}
