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
    std::string particle;
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

struct internalConn {
   // const char* serverPort;
    int serverPort;
    const char* serverIP;
    int socketfd;
    int newSockfd;
    bool ready;
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
        int internalServer();
        int sendSystemInfo(systemStats *ss);
        void updateSystemStats(systemStats *ss, char* buffer);
        void setNodeID(const int id);
        std::string createTestXMLPayload(systemStats *ss);

        //Strucs
        systemStats ss;
        //systemStats *ss = &ss;
        connInfo conn;
        internalConn iConn;
    protected:
    private:
        void setupServerInfo(const char* serverIP, const char* serverPort);
        std::string execAndStore(char* cmd);
};
#endif // CMSCLIENT_H
