#include "../../include/communication/AtomConverter.h"

using namespace com::myselia::communication;

AtomConverter::AtomConverter()
{
	//
}

boost::shared_ptr<Atom> AtomConverter::toAtom(bool value)
{
	boost::shared_ptr<Atom> atom(new Atom("", "boolean"));

	atom->setContent(to_string(value));

	return atom;
}

boost::shared_ptr<Atom> AtomConverter::toAtom(int value)
{
	boost::shared_ptr<Atom> atom(new Atom("", "integer"));

	atom->setContent(to_string(value));

	return atom;
}

boost::shared_ptr<Atom> AtomConverter::toAtom(double value)
{
	boost::shared_ptr<Atom> atom(new Atom("", "double"));

	atom->setContent(to_string(value));

	return atom;
}

boost::shared_ptr<Atom> AtomConverter::toAtom(string value)
{
	boost::shared_ptr<Atom> atom(new Atom("", "string"));

	atom->setContent(value);

	return atom;
}

string AtomConverter::getAsString(boost::shared_ptr<Atom> atom)
{
	if(atom->getAtomClass()!="string")
		throw IllegalArgumentException("Atom is not of string type");

	return atom->getContent();
}

double AtomConverter::getAsDouble(boost::shared_ptr<Atom> atom)
{
	if(atom->getAtomClass()!="double")
		throw IllegalArgumentException("Atom is not of double type");

	return stod(atom->getContent());
}

int AtomConverter::getAsInt(boost::shared_ptr<Atom> atom)
{
	if(atom->getAtomClass()!="integer")
		throw IllegalArgumentException("Atom is not of integer type");

	return stoi(atom->getContent());
}

bool AtomConverter::getAsBool(boost::shared_ptr<Atom> atom)
{
	if(atom->getAtomClass()!="boolean")
		throw IllegalArgumentException("Atom is not of boolean type");

	return atom->getContent()=="true";
}

