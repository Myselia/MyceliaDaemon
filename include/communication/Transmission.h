#ifndef INCLUDE_COMMUNICATION_STRUCTURES_TRANSMISSION_H_
#define INCLUDE_COMMUNICATION_STRUCTURES_TRANSMISSION_H_

#include "../cpp/Generic.h"
#include "Atom.h"

using namespace std;
using namespace com::myselia::cpp;

namespace com
{
namespace myselia
{
namespace communication
{

class Transmission
{
	public:
	Transmission();
	Transmission(string id, string from, string to);

	void setAttributes(string id, string from, string to);
	void addAtom(boost::shared_ptr<Atom> atom);

	void setOpcode(string opcode);
	void setAtoms(list<boost::shared_ptr<Atom>> list);
	void setId(string id);
	void setFrom(string from);
	void setTo(string to);

	string getId();
	string getFrom();
	string getTo();
	string getOpcode();
	list<boost::shared_ptr<Atom>> getAtoms();

	private:
	string id;
	string from;
	string to;
	string opcode;
	list<boost::shared_ptr<Atom>> atoms;
};

}
}
}

#endif /* INCLUDE_COMMUNICATION_STRUCTURES_TRANSMISSION_H_ */
