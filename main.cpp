#include <stdlib.h>
#include <stdio.h>

#include "include/Json.h"

using namespace std;
using namespace json;

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

	(*obj1)["SylvainArray"]=array;
	(*obj1)["Nicolas"]=obj2;

	boost::shared_ptr<JsonPrimitive> primitive6(new JsonPrimitive("very"));

	(*obj2)["Recursive"]=primitive6;

	cout << json.serialize(obj1) << endl;

	return 0;
}
