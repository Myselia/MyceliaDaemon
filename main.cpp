#include <stdlib.h>
#include <stdio.h>

#include <string>

/*#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>*/

#include "include/Json.h"

using namespace std;
using namespace json;

int main(int argc, char** argv)
{
	Json json=Json();

	JsonObject obj1=JsonObject();
	obj1["Eduard"]=JsonNull();
	obj1["LoicBool"]=JsonPrimitive(false);
	obj1["LoicInt"]=JsonPrimitive(0);

	JsonArray array=JsonArray();
	array.add(JsonPrimitive(42));
	array.add(JsonPrimitive("old daemon dude"));
	array.add(JsonPrimitive(true));
	obj1["SylvainArray"]=array;

	obj1["Nicolas"]=JsonObject();

	cout << json.serialize(obj1);

	return 0;
}

