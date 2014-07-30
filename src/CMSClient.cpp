#include "../include/CMSClient.h"

CMSClient::CMSClient(const char* serverIP, const char* serverPort)
{
    conn.serverIP = serverIP;
    conn.serverPort = serverPort;
    memset(&conn.host_info, 0, sizeof conn.host_info);
}

CMSClient::~CMSClient()
{

}

int CMSClient::connectToServer() {
    std::cout << "TEST " << conn.serverIP << std::endl;
    // Struct Setup Protocol
	std::cout << "Setting up the structs..." << std::endl;
	conn.host_info.ai_family = AF_UNSPEC;
	conn.host_info.ai_socktype = SOCK_STREAM;
	conn.status = getaddrinfo(conn.serverIP, conn.serverPort, &conn.host_info, &conn.host_info_list); //"IP", "port", leave it alone, leave it alone
	if(conn.status !=0){
		std::cout << "..STATUS CODE: " << conn.status << std::endl;
		std::cout << "..Err:" << gai_strerror(conn.status) << std::endl;
	} else {
		std::cout << "..Struts setup returned: " << conn.status << "[OK]" << std::endl;
	}

    // Socket Creation Protocol
	std::cout << "Creating a socket..." << std::endl;
	std::cout << "Setting up the socket..." << std::endl;
	socketfd = socket(conn.host_info_list->ai_family, conn.host_info_list->ai_socktype,
		conn.host_info_list->ai_protocol);
	if(socketfd == -1){
		std::cout << "..Socket Error"<< std::endl;
	} else {
		std::cout << "..Socket setup returned: " << socketfd << "[OK]" << std::endl;
	}

    // Socket Connection Protocol
	std::cout << "Connecting to server" << std::endl;
	conn.status = connect(socketfd, conn.host_info_list->ai_addr, conn.host_info_list->ai_addrlen);
	if(conn.status == -1){
		std::cout << "..Connection Error"<< std::endl;
	} else {
		std::cout << "..Connected to host at: " << conn.serverIP << "[OK]" << std::endl;
	}

    return 0;
}

std::string CMSClient::execAndStore(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

std::string CMSClient::createTestXMLPayload(systemStats *ss) {
    std::string payload;
   // payload.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    payload.append("<transmission>");

    payload.append("<header id=\"1\" type=\"sys\" to=\"cms\" from=\"node1\">");
    payload.append("OPCODE");
    payload.append("</header>");

    payload.append("<particle type=\"sys\" class=\"temperature\" count=\"1\">");
    payload.append(ss->systemTemp);
    payload.append("</particle>");

    payload.append("<particle type=\"sys\" class=\"temperature\" count=\"1\">");
    payload.append(ss->systemTemp);
    payload.append("</particle>");

    payload.append("</transmission>\r\n");

    return payload;
}


void CMSClient::updateSystemStats(systemStats *ss) {
    //###The full output of a command ran on the system (assign with execAndStore)
    std::string commandRecv;
    //###Formatted command to comply with conversion to other data types.
    std::string commandFormatted;

    //Update this particular nodes temperature (in C because we arent F[uckheads])
    commandRecv = execAndStore("/opt/vc/bin/vcgencmd measure_temp");
    commandFormatted = commandRecv.substr(5,4);
    ss->systemTemp = commandFormatted;
    //std::cout << "CF: " << commandFormatted << std::endl;
    //ss->systemTemp = ::atof(commandFormatted.c_str());
    std::cout << "SS TEMP VAL: " << ss->systemTemp << std::endl;
}

int CMSClient::sendSystemInfo(systemStats *ss) {

    // Data Sending Protocol
    std::cout << "Sending XML payload...";
    ssize_t bytes_sent;
    std::string payload = createTestXMLPayload(ss);
    std::string *payloadP = &payload;
    std::cout << "MEGA TEST2: " << payload << std::endl;
    //const char* payloadConverted = payload.c_str();
    bytes_sent = send(socketfd, payload.c_str(), strlen(payload.c_str()), 0);
    std::cout << " Sent: " << bytes_sent << std::endl;

    return 0;
}
