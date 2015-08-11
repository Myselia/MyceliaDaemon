#ifndef INCLUDE_CONFIG_H_
#define INCLUDE_CONFIG_H_

#include <cppcommon/Json.h>
#include <cppcommon/IO.h>

using namespace std;
using namespace boost;
using namespace com::myselia::cppcommon;

namespace com
{
namespace myselia
{
namespace daemon
{

class Configuration
{
	public:
	Configuration(string filePath)
	{
		loadConfiguration(filePath);
	}

	string getUsername()
	{
		return username;
	}

	void setUsername(string username)
	{
		this->username = username;
	}

	string getPassword()
	{
		return password;
	}

	void setPassword(string password)
	{
		this->password = password;
	}

	int getPort()
	{
		return port;
	}

	void setPort(int port)
	{
		this->port = port;
	}

	private:
	string username;
	string password;
	int port;

	void loadConfiguration(string filePath)
	{
		try
		{
			vector<char> fileContent=GenericUtil::readAllBytes(filePath);
			std::string fileContentText(fileContent.begin(), fileContent.end());

			boost::shared_ptr<JsonObject> configFile=boost::static_pointer_cast<JsonObject>(Json::parse(fileContentText));

			boost::shared_ptr<JsonPrimitive> usernameJson=boost::static_pointer_cast<JsonPrimitive>((*configFile)["username"]);
			boost::shared_ptr<JsonPrimitive> passwordJson=boost::static_pointer_cast<JsonPrimitive>((*configFile)["password"]);
			boost::shared_ptr<JsonPrimitive> portJson=boost::static_pointer_cast<JsonPrimitive>((*configFile)["port"]);

			username=usernameJson->getAsString();
			password=passwordJson->getAsString();
			port=portJson->getAsInt();
		}
		catch(JsonParseException& exception)
		{
			cout << "Error: error while parsing config file." << endl;

			throw IOException("Error: error while parsing config file.");
		}
	}
};

}
}
}

#endif /* INCLUDE_CONFIG_H_ */