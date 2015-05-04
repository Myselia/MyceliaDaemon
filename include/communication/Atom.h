#ifndef INCLUDE_COMMUNICATION_ATOM_H_
#define INCLUDE_COMMUNICATION_ATOM_H_

#include "../../../MyseliaCppCommon/include/cpp/Generic.h"

using namespace std;
using namespace com::myselia::cpp;

namespace com
{
namespace myselia
{
namespace communication
{

class Atom
{
	public:
	Atom(string atomType, string atomClass);

	void setContent(string content);
	string getContent();
	string getAtomType();
	string getAtomClass();

	private:
	string atomType;
	string atomClass;
	string content;
};

}
}
}

#endif /* INCLUDE_COMMUNICATION_ATOM_H_ */
