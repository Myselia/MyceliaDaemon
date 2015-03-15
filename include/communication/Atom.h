#ifndef INCLUDE_COMMUNICATION_ATOM_H_
#define INCLUDE_COMMUNICATION_ATOM_H_

#include "../../Generic.h"

using namespace std;

namespace mycelia
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

#endif /* INCLUDE_COMMUNICATION_ATOM_H_ */
