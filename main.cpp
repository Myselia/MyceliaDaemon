#include <iostream>		//io
#include <string>
#include <cstdlib>
#include <cstring>		//memset
#include <sys/socket.h>	//socket functions
#include <netdb.h>		//socket functions
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "include/MyceliaDaemon.h"

int main() {
	bool hasConnection = false;
	unsigned int sleepTime = 2000;
	MyceliaDaemon client;

	//Prints out UDP broadcast packets
	client.findServer();
}
