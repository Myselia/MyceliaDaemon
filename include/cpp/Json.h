#ifndef INCLUDE_JSON_H_
#define INCLUDE_JSON_H_

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include "Generic.h"
#include "Exception.h"

using namespace std;

namespace com
{
namespace myselia
{
namespace cpp
{

class JsonParseException: public GenericException
{
	public:
	JsonParseException()
	{
		//Do nothing
	}

	JsonParseException(string message) :
			GenericException(message)
	{
		//Do nothing
	}

	JsonParseException(exception cause) :
			GenericException(cause)
	{
		//Do nothing
	}
};

class JsonElement
{
	public:
	enum JsonElementType
	{
		JsonNull, JsonPrimitive, JsonArray, JsonObject
	};

	bool isJsonNull();
	bool isJsonPrimitive();
	bool isJsonArray();
	bool isJsonObject();

	JsonElementType getJsonElementType();

	protected:
	JsonElement(JsonElementType type);

	private:
	JsonElementType type;
};

class JsonNull: public JsonElement
{
	public:
	JsonNull();
};

class JsonPrimitive: public JsonElement
{
	public:
	enum PrimitiveType
	{
		Bool, Int, String
	};

	JsonPrimitive(bool value);
	JsonPrimitive(int value);
	JsonPrimitive(string value);
	JsonPrimitive(const char* value);
	~JsonPrimitive();

	bool getAsBool() const;
	int getAsInt() const;
	string getAsString() const;

	PrimitiveType getPrimitiveType() const;
	bool isPrimitiveType(PrimitiveType primitiveType) const;

	bool isBool() const;
	bool isInt() const;
	bool isString() const;

	private:
	boost::variant<bool, int, string> val;
};

class JsonArray: public JsonElement
{
	public:
	JsonArray();

	uint size();
	void add(const boost::shared_ptr<JsonElement>& element);

	list<boost::shared_ptr<JsonElement>>::iterator begin();
	list<boost::shared_ptr<JsonElement>>::iterator end();

	private:
	list<boost::shared_ptr<JsonElement>> elements;
};

class JsonObject: public JsonElement
{
	public:
	JsonObject();

	boost::shared_ptr<JsonElement>& operator[](string name);

	unordered_map<string, boost::shared_ptr<JsonElement>>::iterator begin();
	unordered_map<string, boost::shared_ptr<JsonElement>>::iterator end();

	private:
	unordered_map<string, boost::shared_ptr<JsonElement>> map;
};

/**
 * Json serializer and deserializer.
 */
class Json
{
	public:
	Json();

	boost::shared_ptr<JsonElement> parse(string jsonString);
	string serialize(const boost::shared_ptr<JsonElement>& element);

	private:
	boost::shared_ptr<JsonElement> parse(string jsonString, int& position);
	boost::shared_ptr<JsonObject> parseObject(string jsonString, int& position);
	boost::shared_ptr<JsonArray> parseArray(string jsonString, int& position);
	boost::shared_ptr<JsonPrimitive> parsePrimitive(string jsonString, int& position);
	boost::shared_ptr<JsonNull> parseNull(string jsonString, int& position);
	void skipSpaces(string jsonString, int& position);
	char read(string jsonString, int position);
	bool checkEqual(string jsonString, int position, string needle);

	/**
	 * Parse a C string without the delimiters.
	 * This method keeps reading until it hits stopChar.
	 */
	string parseCString(string jsonString, int& position, char stopChar);
	string escapeQuotes(string str);

	string serializeObject(const boost::shared_ptr<JsonObject>& object);
	string serializeArray(const boost::shared_ptr<JsonArray>& array);
	string serializePrimitive(const boost::shared_ptr<JsonPrimitive>& primitive);
	string serializeNull(const boost::shared_ptr<JsonNull>& null);
};

}
}
}

#endif /* INCLUDE_JSON_H_ */
