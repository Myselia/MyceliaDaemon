#include <stdlib.h>
#include <stdio.h>

#include "include/MyceliaDaemon.h"

using namespace std;
using namespace mycelia;


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
