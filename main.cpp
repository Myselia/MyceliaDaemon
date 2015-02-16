#include <iostream>		//io
#include <string>
#include <cstdlib>
#include <cstring>		//memset
#include <sys/socket.h>	//socket functions
#include <netdb.h>		//socket functions
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "include/CMSClient.h"
#include "include/XMLInterpreter.h"
#include "include/rapidxml.hpp"

using namespace rapidxml;
int main() {
	bool hasConnection = false;
	unsigned int sleepTime = 2000;
	CMSClient client;

	//Blocks of test code
	XMLInterpreter xml;

	if (client.findServer() == 0) {
		if (client.connectToServer() == 0) {
			if (client.internalServer() == 0) {

				int n;

				char buffer[255];
				memset(&buffer, 0, sizeof(buffer));

				char internalBuffer[255];
				memset(&internalBuffer, 0, sizeof(buffer));

				while (!client.conn.ready) {

					//  bytes_received = recv(client.conn.socketfd, buffer, 1000, 0);
					unsigned slen = sizeof(sockaddr);
					if (recvfrom(client.conn.socketfd, buffer, sizeof(buffer),
							0, (sockaddr *) &client.conn.host_info, &slen)
							== -1) {
						perror("receive");
					} else {
						printf("recv: %s\n", buffer);
						std::string hold(buffer);

						xml.StringToTrans(hold);
						xml.parseForSystemInfo();

						client.conn.coreID = atoi((xml.getNodeID()).c_str());
						std::cout << "ID of this node is: "
								<< client.conn.coreID << std::endl;

						client.conn.ready = true;
					}
				}

				//NODE DOES THESE THIGNS HERE AFTER CMS ASSIGNS ID
				/*const std::string test = client.createTestXMLPayload(&client.ss);
				 xml.StringToTrans(test);
				 xml_node<> *pRoot = xml.currentDoc.first_node();
				 xml_node<> *pNode = pRoot->first_node();
				 xml_attribute<> *pAttr = pNode->first_attribute();
				 // std::string strValue = pAttr->value();

				 for (xml_attribute<> *attr = pNode->first_attribute();
				 attr; attr = attr->next_attribute())
				 {
				 std::cout << "Node foobar has attribute " << attr->name() << std::endl;
				 std::cout << "with value " << attr->value() << std::endl;
				 }*/

				//std::cout << "STR VALUE FROM XML IS: " << strValue << std::endl;
				for (int i = 0; i < 1000; i++) {
					bzero(&internalBuffer, sizeof(internalBuffer));
					//memset(&internalBuffer, 0, sizeof(buffer));
					n = read(client.iConn.newSockfd, internalBuffer, 255);

					if (n < 0) {
						perror("ERROR reading from socket");
						exit(1);
					}

					//printf("Here is the message: %s\n", internalBuffer);

					client.updateSystemStats(&client.ss, internalBuffer);
					client.sendSystemInfo(&client.ss);
					//sleep(1);
					//usleep(sleepTime);
				}
			} else {
				//std::cout << "Failed to start client with serverIP(" << serverIP << ")" << " on port(" << DEFAULT_PORT << ")" << std::endl;
			}

		} else {

		}
	}
}
