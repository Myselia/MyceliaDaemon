#ifndef CMSCLIENT_H
#define CMSCLIENT_H

#include <iostream>		//io
#include <string>
#include <cstdlib>
#include <cstring>		//memset
#include <sys/socket.h>	//socket functions
#include <netdb.h>		//socket functions
#include <stdio.h>
#include <pthread.h>

struct systemStats {
    std::string systemTemp;
    std::string systemRamRemaining;
    std::string systemCpuUsage;
};

struct connInfo {
    const char* serverPort;
    const char* serverIP;
    int socketfd;
    int coreID;
    int status;
    bool ready;
    struct addrinfo host_info;
	struct addrinfo *host_info_list;
};

class CMSClient
{
    public:
        //Constructors
        CMSClient();
        CMSClient(const char*  serverPort, const char* serverIP);
        virtual ~CMSClient();

        //Functions
        int findServer();
        int connectToServer();
        int sendSystemInfo(systemStats *ss);
        void updateSystemStats(systemStats *ss);
        void setNodeID(const int id);
        std::string createTestXMLPayload(systemStats *ss);

        //Strucs
        systemStats ss;
        //systemStats *ss = &ss;
        connInfo conn;
    protected:
    private:
        void setupServerInfo(const char* serverIP, const char* serverPort);
        std::string execAndStore(char* cmd);
};
#endif // CMSCLIENT_H
