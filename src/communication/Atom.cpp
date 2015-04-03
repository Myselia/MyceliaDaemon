#include "../../include/communication/Atom.h"

using namespace std;
using namespace com::myselia::communication;

Atom::Atom(string atomType, string atomClass):
	atomType(atomType), atomClass(atomClass)
{
	//
}

void Atom::setContent(string content)
{
	this->content=content;
}

string Atom::getContent()
{
	return content;
}

string Atom::getAtomType()
{
	return atomType;
}

string Atom::getAtomClass()
{
	return atomClass;
}

