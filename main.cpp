#include <stdlib.h>
#include <stdio.h>

#include "include/Json.h"

using namespace std;
using namespace mycelia;

void printScale(int len);
char getSecondLastDigit(int num);

int main(int argc, char** argv)
{
	Json json=Json();

	boost::shared_ptr<JsonObject> obj1(new JsonObject());
	boost::shared_ptr<JsonNull> null(new JsonNull());
	boost::shared_ptr<JsonPrimitive> primitive1(new JsonPrimitive(false));
	boost::shared_ptr<JsonPrimitive> primitive2(new JsonPrimitive(0));

	(*obj1)["Eduard"]=null;
	(*obj1)["LoicBool"]=primitive1;
	(*obj1)["LoicInt"]=primitive2;

	boost::shared_ptr<JsonArray> array(new JsonArray());
	boost::shared_ptr<JsonPrimitive> primitive3(new JsonPrimitive(42));
	boost::shared_ptr<JsonPrimitive> primitive4(new JsonPrimitive("old daemon dude"));
	boost::shared_ptr<JsonPrimitive> primitive5(new JsonPrimitive(true));

	array->add(primitive3);
	array->add(primitive4);
	array->add(primitive5);

	boost::shared_ptr<JsonObject> obj2(new JsonObject());
	boost::shared_ptr<JsonObject> obj3(new JsonObject());
	boost::shared_ptr<JsonArray> array2(new JsonArray());

	(*obj1)["SylvainArray"]=array;
	(*obj1)["Nicolas"]=obj2;
	(*obj1)["eobj"]=obj3;
	(*obj1)["earr"]=array2;

	boost::shared_ptr<JsonPrimitive> primitive6(new JsonPrimitive("very"));

	(*obj2)["Recursive"]=primitive6;

	cout << "Serializing..." << endl;
	string jsonstr=json.serialize(obj1);
	printScale(jsonstr.length());
	cout << jsonstr << endl;

	cout << "Parsing..." << endl;
	boost::shared_ptr<JsonElement> parsedJson;
	parsedJson=json.parse(jsonstr);

	cout << "Serializing v2..." << endl;
	jsonstr=json.serialize(parsedJson);
	cout << jsonstr << endl;

	return 0;
}

char getSecondLastDigit(int num)
{
	string str=to_string(num);

	if(str.length()==1)
		return str[0];

	return str[str.length()-2];
}

void printScale(int len)
{
	for(int i=0; i<len; i++)
	{
		if(i%10==0)
			cout << getSecondLastDigit(i);
		else if(i%5==0)
			cout << "+";
		else
			cout << "-";
	}

	cout << endl;
}
