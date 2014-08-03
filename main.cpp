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
            std::cout << "Client started with serverIP(" << serverIP << ")" << " on port(" << DEFAULT_PORT << ")" << std::endl;

            for (int i = 0; i < 1000; i ++)
            {
                client.updateSystemStats(&client.ss);
                client.sendSystemInfo(&client.ss);
                std::cout << "DOING SHIT" << std::endl;
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


    //Once the server info has been gathered, use it to connect to the server

    //systemStats ss;
    //updateSystemStats(&ss);
    // std::cout << createTestXMLPayload(&ss) << std::endl;


}

/*int status;
struct addrinfo host_info;
struct addrinfo *host_info_list;
memset(&host_info, 0, sizeof host_info);

// Struct Setup Protocol
std::cout << "Setting up the structs..." << std::endl;
host_info.ai_family = AF_UNSPEC;
host_info.ai_socktype = SOCK_STREAM;
status = getaddrinfo(serverIP, DEFAULT_PORT, &host_info, &host_info_list); //-------------------------------------"IP", "port"
if(status !=0){
	std::cout << "..STATUS CODE: " << status << std::endl;
	std::cout << "..Err:" << gai_strerror(status) << std::endl;
} else {
	std::cout << "..Struts setup returned: " << status << "[OK]" << std::endl;
}

// Socket Creation Protocol
//std::cout << "Creating a socket..." << std::endl;
int socketfd;
std::cout << "Setting up the socket..." << std::endl;
socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
	host_info_list->ai_protocol);
if(socketfd == -1){
	std::cout << "..Socket Error"<< std::endl;
} else {
	std::cout << "..Socket setup returned: " << socketfd << "[OK]" << std::endl;
}


// Socket Connection Protocol
std::cout << "Connecting to server" << std::endl;
status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
if(status == -1){
	std::cout << "..Connection Error"<< std::endl;
} else {
	std::cout << "..Connected to host at: " << serverIP << "[OK]" << std::endl;
}



// Data Sending Protocol
std::cout << "Sending...";
ssize_t bytes_sent;
std::string payload = createTestXMLPayload(&ss);
std::string *payloadP = &payload;
std::cout << "MEGA TEST2: " << payload << std::endl;
//const char* payloadConverted = payload.c_str();
bytes_sent = send(socketfd, payload.c_str(), strlen(payload.c_str()), 0);
std::cout << " Sent: " << bytes_sent << std::endl;

// Data Receiving Protocol
std::cout << "Receiving..." << std::endl;
ssize_t bytes_received;
char incoming_data_buffer[1000];
bytes_received = recv(socketfd, incoming_data_buffer, 1000, 0);
if(bytes_received == 0) std::cout << "Host Shut Down" << std::endl;
if(bytes_received == -1) std::cout << "Recieve Error" << std::endl;
std::cout << bytes_received << " bytes received" << std::endl;
std::cout << incoming_data_buffer << std::endl;

// Socket Closing Protocol
std::cout << "Receiving complete. Closing socket..." << std::endl;
freeaddrinfo(host_info_list);
close(socketfd);

std::cout << "End" <<std::endl;
return 0;
*/


