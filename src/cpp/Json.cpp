#include "../../include/cpp/Json.h"

using namespace std;
using namespace com::myselia::cpp;

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

JsonElement::JsonElementType JsonElement::getJsonElementType()
{
	return type;
}

//-------------------JsonNull-------------------

JsonNull::JsonNull() :
		JsonElement(JsonElementType::JsonNull)
{
	//Do nothing
}

//-------------------JsonPrimitive-------------------

JsonPrimitive::JsonPrimitive(bool value) :
		JsonElement(JsonElementType::JsonPrimitive)
{
	val=value;
}

JsonPrimitive::JsonPrimitive(int value) :
		JsonElement(JsonElementType::JsonPrimitive)
{
	val=value;
}

JsonPrimitive::JsonPrimitive(string value) :
		JsonElement(JsonElementType::JsonPrimitive)
{
	val=value;
}

JsonPrimitive::JsonPrimitive(const char* value) :
		JsonElement(JsonElementType::JsonPrimitive)
{
	val=(string)value;
}

JsonPrimitive::~JsonPrimitive()
{
	//Do Nothing
}

bool JsonPrimitive::getAsBool() const
{
	if( !isBool())
		throw IllegalStateException("Value not a boolean");

	return boost::get<bool>(val);
}

int JsonPrimitive::getAsInt() const
{
	if( !isInt())
		throw IllegalStateException("Value not an integer");

	return boost::get<int>(val);
}

string JsonPrimitive::getAsString() const
{
	if( !isString())
		throw IllegalStateException("Value not a string");

	return boost::get<string>(val);
}

bool JsonPrimitive::isBool() const
{
	return isPrimitiveType(PrimitiveType::Bool);
}

bool JsonPrimitive::isInt() const
{
	return isPrimitiveType(PrimitiveType::Int);
}

bool JsonPrimitive::isString() const
{
	return isPrimitiveType(PrimitiveType::String);
}

JsonPrimitive::PrimitiveType JsonPrimitive::getPrimitiveType() const
{
	if(val.type()==typeid(bool))
		return PrimitiveType::Bool;
	else if(val.type()==typeid(int))
		return PrimitiveType::Int;
	else
		return PrimitiveType::String;
}

bool JsonPrimitive::isPrimitiveType(PrimitiveType primitiveType) const
		{
	return getPrimitiveType()==primitiveType;
}

//-------------------JsonArray-------------------

JsonArray::JsonArray() :
		JsonElement(JsonElementType::JsonArray)
{
	elements=list<boost::shared_ptr<JsonElement>>();
}

uint JsonArray::size()
{
	return elements.size();
}

void JsonArray::add(const boost::shared_ptr<JsonElement>& element)
{
	elements.push_back(element);
}

list<boost::shared_ptr<JsonElement>>::iterator JsonArray::begin()
{
	return elements.begin();
}

list<boost::shared_ptr<JsonElement>>::iterator JsonArray::end()
{
	return elements.end();
}

//-------------------JsonObject-------------------

JsonObject::JsonObject() :
		JsonElement(JsonElementType::JsonObject)
{
	map=unordered_map<string, boost::shared_ptr<JsonElement>>();
}

boost::shared_ptr<JsonElement>& JsonObject::operator[](string name)
{
	boost::shared_ptr<com::myselia::cpp::JsonNull> null(new com::myselia::cpp::JsonNull);

	return ( *((map.insert(unordered_map<string, boost::shared_ptr<JsonElement>>::value_type(name, null))).first)).second;
}

unordered_map<string, boost::shared_ptr<JsonElement>>::iterator JsonObject::begin()
{
	return map.begin();
}

unordered_map<string, boost::shared_ptr<JsonElement>>::iterator JsonObject::end()
{
	return map.end();
}

//-------------------Json-------------------
Json::Json()
{
	//Do nothing
}

boost::shared_ptr<JsonElement> Json::parse(string jsonString)
{
	int position=0;

	return parse(jsonString, position);
}

string Json::serialize(const boost::shared_ptr<JsonElement>& element)
{
	if(element->isJsonNull())
		return serializeNull(boost::static_pointer_cast<JsonNull>(element));
	else if(element->isJsonPrimitive())
		return serializePrimitive(boost::static_pointer_cast<JsonPrimitive>(element));
	else if(element->isJsonArray())
		return serializeArray(boost::static_pointer_cast<JsonArray>(element));
	else
		return serializeObject(boost::static_pointer_cast<JsonObject>(element));
}

boost::shared_ptr<JsonElement> Json::parse(string jsonString, int& position)
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

	for(int i=0; i<needle.length(); i++)
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
				message+=to_string(position-1);

				throw JsonParseException(message);
			}

			escapeCode=false;
		}
		else
		{
			if(ch=='\\')
				escapeCode=true;
			else if(ch=='\n')
				throw JsonParseException("Illegal newline ('\\n') in string as position "+to_string(position));
			else
				str+=ch;
		}

		position++;
		ch=read(jsonString, position);
	}

	return str;
}

string Json::escapeQuotes(string str)
{
	string ret="";

	for(int i=0; i<str.length(); i++)
	{
		if(str[i]=='"')
			ret+="\\\"";
		else
			ret+=str[i];
	}

	return ret;
}

boost::shared_ptr<JsonObject> Json::parseObject(string jsonString, int& position)
{
	if(read(jsonString, position)!='{')
		throw JsonParseException("Expected object at position "+to_string(position));

	position++;

	boost::shared_ptr<JsonObject> object(new JsonObject());

	if( !checkEqual(jsonString, position, "}"))
	{
		while(true)
		{
			skipSpaces(jsonString, position);

			//Parse key
			if( !checkEqual(jsonString, position, "\""))
				throw JsonParseException("Expected object key (string starting with '\"') at position "+to_string(position));

			position++;
			string key=parseCString(jsonString, position, '"');
			position++;

			//Parse value
			skipSpaces(jsonString, position);

			if( !checkEqual(jsonString, position, ":"))
				throw JsonParseException("Expected ':' at position "+to_string(position));
			position++;

			boost::shared_ptr<JsonElement> value=parse(jsonString, position);

			//Add property to object
			( *object)[key]=value;

			skipSpaces(jsonString, position);

			if(checkEqual(jsonString, position, "}"))
				break;

			if(read(jsonString, position)!=',')
				throw JsonParseException("Expected property delimiter (',') at position "+to_string(position));

			position++;
		}
	}

	position++;

	return object;
}

boost::shared_ptr<JsonArray> Json::parseArray(string jsonString, int& position)
{
	if(read(jsonString, position)!='[')
		throw JsonParseException("Expected array at position "+to_string(position));

	position++;

	boost::shared_ptr<JsonArray> array(new JsonArray());

	if( !checkEqual(jsonString, position, "]"))
	{
		while(true)
		{
			skipSpaces(jsonString, position);

			array->add(parse(jsonString, position));

			skipSpaces(jsonString, position);

			if(checkEqual(jsonString, position, "]"))
				break;

			if( !checkEqual(jsonString, position, ","))
				throw JsonParseException("Expected array element delimiter (',') at position "+to_string(position));

			position++;
		}
	}

	position++;

	return array;
}

boost::shared_ptr<JsonPrimitive> Json::parsePrimitive(string jsonString, int& position)
{
	//primitives: true, false, 'x', "asd", 4

	int const start=position;

	if(checkEqual(jsonString, position, "true"))
	{
		position+=4;

		boost::shared_ptr<JsonPrimitive> primitive(new JsonPrimitive(true));

		return primitive;
	}
	else if(checkEqual(jsonString, position, "false"))
	{
		position+=5;

		boost::shared_ptr<JsonPrimitive> primitive(new JsonPrimitive(false));

		return primitive;
	}
	else if(read(jsonString, position)=='\'')
	{
		position++;
		string value=parseCString(jsonString, position, '\'');
		position++;

		if(value.length()>1)
			throw JsonParseException("Character declaration too long at position "+start);

		boost::shared_ptr<JsonPrimitive> primitive(new JsonPrimitive(value[0]));

		return primitive;
	}
	else if(read(jsonString, position)=='"')
	{
		position++;
		string value=parseCString(jsonString, position, '"');
		position++;

		boost::shared_ptr<JsonPrimitive> primitive(new JsonPrimitive(value));

		return primitive;
	}
	else
	{
		char ch=read(jsonString, position);

		if(ch<'0'||ch>'9')
			throw JsonParseException("Expected integer at position "+to_string(start));

		string str="";

		while(ch>='0'&&ch<='9')
		{
			str+=ch;
			position++;
			ch=read(jsonString, position);
		}

		boost::shared_ptr<JsonPrimitive> primitive(new JsonPrimitive(stoi(str)));

		return primitive;
	}
}

boost::shared_ptr<JsonNull> Json::parseNull(string jsonString, int& position)
{
	if( !checkEqual(jsonString, position, "null"))
		throw JsonParseException("Expected \"null\" at position "+to_string(position));

	position+=4;

	boost::shared_ptr<JsonNull> null(new JsonNull());

	return null;
}

string Json::serializeObject(const boost::shared_ptr<JsonObject>& object)
{
	string str="{";
	unordered_map<string, boost::shared_ptr<JsonElement>>::iterator it;
	bool first=true;

	for(it=object->begin(); it!=object->end(); it++)
	{
		if(first)
			first=false;
		else
			str+=',';

		str+="\""+( *it).first+"\":"+serialize(( *it).second);
	}

	str+="}";

	return str;
}

string Json::serializeArray(const boost::shared_ptr<JsonArray>& array)
{
	string str="[";
	list<boost::shared_ptr<JsonElement>>::iterator it;
	bool first=true;

	for(it=array->begin(); it!=array->end(); it++)
	{
		if(first)
			first=false;
		else
			str+=',';

		str+=serialize( *it);
	}

	str+="]";

	return str;
}

string Json::serializePrimitive(const boost::shared_ptr<JsonPrimitive>& primitive)
{
	primitive->isBool();

	if(primitive->isBool())
	{
		if(primitive->getAsBool())
			return "true";
		else
			return "false";
	}
	else if(primitive->isInt())
		return to_string(primitive->getAsInt());
	else
		return "\""+escapeQuotes(primitive->getAsString())+"\"";
}

string Json::serializeNull(const boost::shared_ptr<JsonNull>& null)
{
	return "null";
}

