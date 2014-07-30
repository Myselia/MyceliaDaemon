#ifndef CMSCLIENT_H
#define CMSCLIENT_H

#include <iostream>		//io
#include <string>
#include <cstdlib>
#include <cstring>		//memset
#include <sys/socket.h>	//socket functions
#include <netdb.h>		//socket functions
#include <stdio.h>

struct systemStats {
    std::string systemTemp;
};

struct connInfo {
    const char* serverPort;
    const char* serverIP;
    int status;
    struct addrinfo host_info;
	struct addrinfo *host_info_list;
};

class CMSClient
{
    public:
        //Constructors
        CMSClient(const char*  serverPort, const char* serverIP);
        virtual ~CMSClient();

        //Functions
        std::string createTestXMLPayload(systemStats *ss);
        int connectToServer();
        int sendSystemInfo(systemStats *ss);
        void updateSystemStats(systemStats *ss);

        //Strucs
        systemStats ss;
        //systemStats *ss = &ss;
        connInfo conn;

        //Vars
        int socketfd;
    protected:
    private:
        std::string execAndStore(char* cmd);
};

#endif // CMSCLIENT_H
