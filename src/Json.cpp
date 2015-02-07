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
