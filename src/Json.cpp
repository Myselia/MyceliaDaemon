#include "../include/Json.h"

using namespace Json;

//-------------------JsonElement-------------------

JsonElement::JsonElement(JsonElementType type)
{
	this->type=type;
}

bool JsonElement::isJsonNull()
{
	return type==JsonElementType::JsonNull;
}

bool JsonElement::isJsonPrimitive()
{
	return type==JsonElementType::JsonPrimitive;
}

bool JsonElement::isJsonArray()
{
	return type==JsonElementType::JsonArray;
}

bool JsonElement::isJsonObject()
{
	return type==JsonElementType::JsonObject;
}

//-------------------JsonNull-------------------

JsonNull::JsonNull(): JsonElement(JsonElementType::JsonNull)
{
	//Do nothing
}

//-------------------JsonPrimitive-------------------

JsonPrimitive::JsonPrimitive(bool value): JsonElement(JsonElementType::JsonPrimitive)
{
	primitiveType=PrimitiveType::Bool;
	bval=value;
}

JsonPrimitive::JsonPrimitive(char value): JsonElement(JsonElementType::JsonPrimitive)
{
	primitiveType=PrimitiveType::Char;
	cval=value;
}

JsonPrimitive::JsonPrimitive(int value): JsonElement(JsonElementType::JsonPrimitive)
{
	primitiveType=PrimitiveType::Int;
	ival=value;
}

JsonPrimitive::JsonPrimitive(string value): JsonElement(JsonElementType::JsonPrimitive)
{
	primitiveType=PrimitiveType::String;
	sval=value;
}

bool JsonPrimitive::getAsBool()
{
	if(primitiveType!=PrimitiveType::Bool)
		throw IllegalStateException("Value not a boolean");

	return bval;
}

int JsonPrimitive::getAsInt()
{
	if(primitiveType!=PrimitiveType::Int)
		throw IllegalStateException("Value not an integer");

	return ival;
}

string JsonPrimitive::getAsString()
{
	if(primitiveType!=PrimitiveType::String)
		throw IllegalStateException("Value not an string");

	return sval;
}

char JsonPrimitive::getAsChar()
{
	if(primitiveType!=PrimitiveType::Char)
		throw IllegalStateException("Value not a character");

	return cval;
}

bool JsonPrimitive::isBool()
{
	return primitiveType==PrimitiveType::Bool;
}

bool JsonPrimitive::isInt()
{
	return primitiveType==PrimitiveType::Int;
}

bool JsonPrimitive::isString()
{
	return primitiveType==PrimitiveType::String;
}

bool JsonPrimitive::isChar()
{
	return primitiveType==PrimitiveType::Char;
}

JsonPrimitive::PrimitiveType JsonPrimitive::getPrimitiveType()
{
	return primitiveType;
}

bool JsonPrimitive::isPrimitiveType(PrimitiveType primitiveType)
{
	return this->primitiveType==primitiveType;
}

//-------------------JsonArray-------------------

JsonArray::JsonArray(): JsonElement(JsonElementType::JsonArray)
{
	elements=list();
}

uint JsonArray::size()
{
	return elements.size();
}

void JsonArray::add(JsonElement element)
{
	elements.push_back(element);
}

list<JsonElement>::iterator JsonArray::begin()
{
	return elements.begin();
}

list<JsonElement>::iterator JsonArray::end()
{
	return elements.end();
}

//-------------------JsonObject-------------------

JsonObject::JsonObject(): JsonElement(JsonElementType::JsonObject)
{
	map=unordered_map();
}

JsonElement& JsonObject::operator[](string name)
{
	return map[name];
}

//-------------------Json-------------------
Json::Json()
{
	//Do nothing
}

JsonElement Json::parse(string jsonString)
{
	int position=0;

	return parse(jsonString, position);
}

string Json::serialize(JsonElement element)
{
	//
}

JsonElement Json::parse(string jsonString, int& position)
{
	skipSpaces(jsonString, position);

	char ch=read(jsonString, position);

	if(ch=='{')
		return parseObject(jsonString, position);
	else if(ch=='[')
		return parseArray(jsonString, position);
	else if(ch=='n')
		return parseNull(jsonString, position);
	else
		return parsePrimitive(jsonString, position);
}

void Json::skipSpaces(string jsonString, int& position)
{
	char ch=read(jsonString, position);

	while(ch==' '||ch=='\t'||ch=='\r'||ch=='\n')
	{
		position++;
		ch=read(jsonString, position);
	}
}

char Json::read(string jsonString, int position)
{
	if(position>=jsonString.length())
		throw new JsonParseException("Not enough characters");

	return jsonString[position];
}

bool Json::checkEqual(string jsonString, int position, string needle)
{
	string str="";

	for(int i=0; i<=needle.length(); i++)
		str+=read(jsonString, position+i);

	return needle==str;
}

string Json::parseCString(string jsonString, int& position, char stopChar)
{
	string str="";

	char ch=read(jsonString, position);
	bool escapeCode=false;

	while(ch!=stopChar)
	{
		if(escapeCode)
		{
			if(ch=='\\')
				str+="\\";
			else if(ch=='t')
				str+="\t";
			else if(ch=='r')
				str+="\r";
			else if(ch=='n')
				str+="\n";

			escapeCode=false;
		}
		else
		{
			if(ch=='\\')
				escapeCode=true;
			else if(ch=='\n')
				throw new JsonParseException("Illegal newline ('\\n') in string as position "+position);
			else
				str+=ch;
		}

		position++;
		ch=read(jsonString, position);
	}

	return str;
}

JsonObject Json::parseObject(string jsonString, int& position)
{
	if(read(jsonString, position)!='{')
		throw new JsonParseException("Expected object at position "+position);

	position++;

	JsonObject object=JsonObject();

	while(read(jsonString, position)!='}')
	{
		skipSpaces(jsonString, position);

		//Parse key
		if(!checkEqual(jsonString, position, "\""))
			throw new JsonParseException("Expected object key (string starting with '\"') at position "+position);

		position++;
		string key=parseCString(jsonString, position, '"');
		position++;

		//Parse value
		skipSpaces(jsonString, position);
		JsonElement value=parse(jsonString, position);

		//Add property to object
		object[key]=value;

		skipSpaces(jsonString, position);

		if(checkEqual(jsonString, position, ","))
			throw new JsonParseException("Expected array element delimiter (',') at position "+position);

		position++;
	}

	position++;

	return array;
}

JsonArray Json::parseArray(string jsonString, int& position)
{
	if(read(jsonString, position)!='[')
		throw new JsonParseException("Expected array at position "+position);

	position++;

	JsonArray array=JsonArray();

	while(read(jsonString, position)!=']')
	{
		skipSpaces(jsonString, position);

		array.add(parse(jsonString, position));

		skipSpaces(jsonString, position);

		if(checkEqual(jsonString, position, ","))
			throw new JsonParseException("Expected array element delimiter (',') at position "+position);

		position++;
	}

	position++;

	return array;
}

JsonPrimitive Json::parsePrimitive(string jsonString, int& position)
{
	//primitives: true, false, 'x', "asd", 4

	int const start=position;

	if(checkEqual(jsonString, position, "true"))
	{
		position+=4;
		return new JsonPrimitive(true);
	}
	else if(checkEqual(jsonString, position, "false"))
	{
		position+=5;
		return new JsonPrimitive(false);
	}
	else if(read(jsonString, position)=='\'')
	{
		position++;
		string value=parseCString(jsonString, position, '\'');
		position++;

		if(value.length()>1)
			throw new JsonParseException("Character declaration too long at position "+start);

		return JsonPrimitive(value[0]);
	}
	else if(read(jsonString, position)=='"')
	{
		position++;
		string value=parseCString(jsonString, position, '"');
		position++;

		return JsonPrimitive(value);
	}
	else
	{
		char ch=read(jsonString, position);

		if(ch<'0'||ch>'9')
			throw new JsonParseException("Expected integer at position "+start);

		string str="";

		while(ch>='0'&&ch<='9')
		{
			str+=ch;
			position++;
			ch=read(jsonString, position);
		}

		return JsonPrimitive(atoi((char const*)str));
	}
}

JsonNull Json::parseNull(string jsonString, int& position)
{
	if(!checkEqual(jsonString, position, "null"))
		throw new JsonParseException("Expected \"null\" at position "+position);

	position+=4;

	return new JsonNull();
}

string Json::serializeObject(JsonObject object)
{
	//
}

string Json::serializeArray(JsonArray array)
{
	//
}

string Json::serializePrimitive(JsonPrimitive primitive)
{
	//
}

string Json::serializeNull(JsonNull null)
{
	//
}

