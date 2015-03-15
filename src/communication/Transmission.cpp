#include "../../include/communication/Transmission.h"

using namespace std;
using namespace mycelia;

Transmission::Transmission()
{
	//
}

Transmission::Transmission(string id, string from, string to):
		id(id), from(from), to(to)
{
	//
}

void Transmission::setAttributes(string id, string from, string to)
{
	this->id=id;
	this->from=from;
	this->to=to;
}

void Transmission::addAtom(boost::shared_ptr<Atom> atom)
{
	atoms.push_back(atom);
}

void Transmission::setOpcode(string opcode)
{
	this->opcode=opcode;
}

void Transmission::setAtoms(list<boost::shared_ptr<Atom>> list)
{
	atoms=list;
}

void Transmission::setId(string id)
{
	this->id=id;
}

void Transmission::setFrom(string from)
{
	this->from=from;
}

void Transmission::setTo(string to)
{
	this->to=to;
}

string Transmission::getId()
{
	return id;
}

string Transmission::getFrom()
{
	return from;
}

string Transmission::getTo()
{
	return to;
}

string Transmission::getOpcode()
{
	return opcode;
}

list<boost::shared_ptr<Atom>> Transmission::getAtoms()
{
	return atoms;
}
