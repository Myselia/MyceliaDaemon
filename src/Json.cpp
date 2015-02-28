#include "../include/Json.h"

using namespace json;

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


JsonPrimitive::JsonPrimitive(const JsonPrimitive& primitive): JsonElement(JsonElementType::JsonPrimitive)
{
	primitiveType=primitive.getPrimitiveType();

	if(primitive.isBool())
		bval=primitive.getAsBool();
	else if(primitive.isInt())
		ival=primitive.getAsInt();
	else
		sval=primitive.getAsString();
}

JsonPrimitive::~JsonPrimitive()
{
	//Do Nothing
}

bool JsonPrimitive::getAsBool() const
{
	if(primitiveType!=PrimitiveType::Bool)
		throw IllegalStateException("Value not a boolean");

	return bval;
}

int JsonPrimitive::getAsInt() const
{
	if(primitiveType!=PrimitiveType::Int)
		throw IllegalStateException("Value not an integer");

	return ival;
}

string JsonPrimitive::getAsString() const
{
	if(primitiveType!=PrimitiveType::String)
		throw IllegalStateException("Value not a string");

	return sval;
}

bool JsonPrimitive::isBool() const
{
	return primitiveType==PrimitiveType::Bool;
}

bool JsonPrimitive::isInt() const
{
	return primitiveType==PrimitiveType::Int;
}

bool JsonPrimitive::isString() const
{
	return primitiveType==PrimitiveType::String;
}

JsonPrimitive::PrimitiveType JsonPrimitive::getPrimitiveType() const
{
	return primitiveType;
}

bool JsonPrimitive::isPrimitiveType(PrimitiveType primitiveType) const
{
	return this->primitiveType==primitiveType;
}

//-------------------JsonArray-------------------

JsonArray::JsonArray(): JsonElement(JsonElementType::JsonArray)
{
	elements=list<JsonElement>();
}

uint JsonArray::size()
{
	return elements.size();
}

void JsonArray::add(JsonElement& element)
{
	elements.push_back(element);
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
	map=unordered_map<string, JsonElement>();
}

JsonElement& JsonObject::operator[](string name)
{
	return (*((map.insert(unordered_map<string, JsonElement>::value_type(name, json::JsonNull()))).first)).second;
}

unordered_map<string, JsonElement>::iterator JsonObject::begin()
{
	return map.begin();
}

unordered_map<string, JsonElement>::iterator JsonObject::end()
{
	return map.end();
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

string Json::serialize(JsonElement& element)
{
	if(element.isJsonNull())
		return serializeNull((JsonNull&)element);
	else if(element.isJsonPrimitive())
		return serializePrimitive((JsonPrimitive&)element);
	else if(element.isJsonArray())
			return serializeArray((JsonArray&)element);
	else
		return serializeObject((JsonObject&)element);
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
		throw JsonParseException("Not enough characters");

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
			else
			{
				string message="";

				message+="Unknown escape code '\\";
				message+=ch;
				message+="' at position ";
				message+=(position-1);

				throw JsonParseException(message);
			}


			escapeCode=false;
		}
		else
		{
			if(ch=='\\')
				escapeCode=true;
			else if(ch=='\n')
				throw JsonParseException("Illegal newline ('\\n') in string as position "+position);
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
		throw JsonParseException("Expected object at position "+position);

	position++;

	JsonObject object=JsonObject();

	while(read(jsonString, position)!='}')
	{
		skipSpaces(jsonString, position);

		//Parse key
		if(!checkEqual(jsonString, position, "\""))
			throw JsonParseException("Expected object key (string starting with '\"') at position "+position);

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
			throw JsonParseException("Expected array element delimiter (',') at position "+position);

		position++;
	}

	position++;

	return object;
}

JsonArray Json::parseArray(string jsonString, int& position)
{
	if(read(jsonString, position)!='[')
		throw JsonParseException("Expected array at position "+position);

	position++;

	JsonArray array=JsonArray();

	while(read(jsonString, position)!=']')
	{
		skipSpaces(jsonString, position);

		array.add(parse(jsonString, position));

		skipSpaces(jsonString, position);

		if(checkEqual(jsonString, position, ","))
			throw JsonParseException("Expected array element delimiter (',') at position "+position);

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

		return JsonPrimitive(true);
	}
	else if(checkEqual(jsonString, position, "false"))
	{
		position+=5;
		return JsonPrimitive(false);
	}
	else if(read(jsonString, position)=='\'')
	{
		position++;
		string value=parseCString(jsonString, position, '\'');
		position++;

		if(value.length()>1)
			throw JsonParseException("Character declaration too long at position "+start);

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
			throw JsonParseException("Expected integer at position "+start);

		string str="";

		while(ch>='0'&&ch<='9')
		{
			str+=ch;
			position++;
			ch=read(jsonString, position);
		}

		return JsonPrimitive(stoi(str));
	}
}

JsonNull Json::parseNull(string jsonString, int& position)
{
	if(!checkEqual(jsonString, position, "null"))
		throw JsonParseException("Expected \"null\" at position "+position);

	position+=4;

	return JsonNull();
}

string Json::serializeObject(JsonObject& object)
{
	string str="{";
	unordered_map<string, JsonElement>::iterator it;

	for(it=object.begin(); it!=object.end(); it++)
		str+="\""+(*it).first+"\":"+serialize((*it).second)+",";

	str+="}";

	return str;
}

string Json::serializeArray(JsonArray& array)
{
	string str="[";
	list<JsonElement>::iterator it;

	for(it=array.begin(); it!=array.end(); it++)
		str+=serialize(*it)+",";

	str+="]";

	return str;
}

string Json::serializePrimitive(JsonPrimitive& primitive)
{
	if(primitive.isBool())
	{
		if(primitive.getAsBool())
			return "true";
		else
			return "false";
	}
	else if(primitive.isInt())
		return to_string(primitive.getAsInt());
	else
		return primitive.getAsString();
}

string Json::serializeNull(JsonNull& null)
{
	return "null";
}

