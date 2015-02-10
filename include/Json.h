#ifndef INCLUDE_JSON_H_
#define INCLUDE_JSON_H_

#include <string>

#include "Generic.h"
#include "Exception.h"

using namespace std;

namespace Json
{

class JsonParseException: GenericException
{
	public:
	JsonParseException()
	{
		//Do nothing
	}

	JsonParseException(string message): GenericException(message)
	{
		//Do nothing
	}

	JsonParseException(exception cause): GenericException(cause)
	{
		//Do nothing
	}
};

class JsonElement
{
	public:
		enum JsonElementType {JsonNull, JsonPrimitive, JsonArray, JsonObject};

		bool isJsonNull();
		bool isJsonPrimitive();
		bool isJsonArray();
		bool isJsonObject();

	protected:
		JsonElement(JsonElementType type);

	private:
		JsonElementType type;
};

class JsonNull: JsonElement
{
	public:
		JsonNull();
};

class JsonPrimitive: JsonElement
{
	public:
		enum PrimitiveType{Bool, Char, Int, String};

		JsonPrimitive(bool value);
		JsonPrimitive(char value);
		JsonPrimitive(int value);
		JsonPrimitive(string value);

		bool getAsBool();
		int getAsInt();
		string getAsString();
		char getAsChar();

		PrimitiveType getPrimitiveType();
		bool isPrimitiveType(PrimitiveType primitiveType);

		bool isBool();
		bool isInt();
		bool isString();
		bool isChar();

	private:
		PrimitiveType primitiveType;

		union
		{
			bool bval;
			char cval;
			int ival;
			string sval;
		};
};

class JsonArray: JsonElement
{
	public:
		JsonArray();

		uint size();
		void add(JsonElement element);

		list<JsonElement>::iterator begin();
		list<JsonElement>::iterator end();

	private:
		list<JsonElement> elements;
};

class JsonObject: JsonElement
{
	public:
		JsonObject();

		JsonElement& operator[](string name);

	private:
		unordered_map<string, JsonElement>  map;
};

/**
 * Json serializer and deserializer.
 */
class Json
{
	public:
		Json();

		JsonElement parse(string jsonString);
		string serialize(JsonElement element);

	private:
		JsonElement parse(string jsonString, int& position);
		JsonObject parseObject(string jsonString, int& position);
		JsonArray parseArray(string jsonString, int& position);
		JsonPrimitive parsePrimitive(string jsonString, int& position);
		JsonNull parseNull(string jsonString, int& position);
		void skipSpaces(string jsonString, int& position);
		char read(string jsonString, int position);
		bool checkEqual(string jsonString, int position, string needle);

		/**
		 * Parse a C string without the delimiters.
		 * This method keeps reading until it hits stopChar.
		 */
		string parseCString(string jsonString, int& position, char stopChar);

		string serializeObject(JsonObject object);
		string serializeArray(JsonArray array);
		string serializePrimitive(JsonPrimitive primitive);
		string serializeNull(JsonNull null);
};

}

#endif /* INCLUDE_JSON_H_ */
