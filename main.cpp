#include <iostream>		//io
#include <string>
#include <cstdlib>
#include <cstring>		//memset
#include <sys/socket.h>	//socket functions
#include <netdb.h>		//socket functions
#include <stdio.h>
#include <pthread.h>

#include "include/CMSClient.h"


const char* serverIP = "132.205.84.236";
const char* DEFAULT_PORT = "6969";

int main()
{
    bool hasConnection = false;
    CMSClient client;


    if (client.findServer() == 0)
    {
        if (client.connectToServer() == 0)
        {
            //On when first connected, the client will wait for ID and role assignments from the CMS
            std::cout << "Client started with serverIP(" << serverIP << ")" << " on port(" << DEFAULT_PORT << ")" << std::endl;

            char buffer[255];
            memset(&buffer, 0, sizeof(buffer));
            //ssize_t bytes_received;

            while (!client.conn.ready)
            {
              //  bytes_received = recv(client.conn.socketfd, buffer, 1000, 0);
                unsigned slen = sizeof(sockaddr);
                if (recvfrom(client.conn.socketfd, buffer, sizeof(buffer), 0, (sockaddr *)&client.conn.host_info, &slen)==-1)
                {
                    perror("receive");
                }
                else
                {
                    printf("recv: %s\n", buffer);
                    std::string hold(buffer);
                    client.conn.coreID = atoi(hold.c_str());
                    std::cout << "ID of this node is: " << client.conn.coreID << std::endl;
                    client.conn.ready = true;
                }
            }

            for (int i = 0; i < 1000; i ++)
            {
                client.updateSystemStats(&client.ss);
                client.sendSystemInfo(&client.ss);
                sleep(1);
            }
        }
        else
        {
            std::cout << "Failed to start client with serverIP(" << serverIP << ")" << " on port(" << DEFAULT_PORT << ")" << std::endl;
        }

    }
    else
    {

    }

}
