#include "../../include/cpp/Generic.h"

using namespace std;
using namespace com::myselia::cpp;

char GenericUtil::getSecondLastDigit(int num)
{
	string str=to_string(num);

	if(str.length()==1)
		return str[0];

	return str[str.length()-2];
}

void GenericUtil::printScale(int len)
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
