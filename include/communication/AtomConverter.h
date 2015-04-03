#ifndef INCLUDE_COMMUNICATION_ATOMCONVERTER_H_
#define INCLUDE_COMMUNICATION_ATOMCONVERTER_H_

#include "../cpp/Generic.h"
#include "../cpp/Exception.h"
#include "Atom.h"

using namespace std;
using namespace com::myselia::cpp;

namespace com
{
namespace myselia
{
namespace communication
{

class AtomConverter
{
	public:
	AtomConverter();

	boost::shared_ptr<Atom> toAtom(bool value);
	boost::shared_ptr<Atom> toAtom(int value);
	boost::shared_ptr<Atom> toAtom(double value);
	boost::shared_ptr<Atom> toAtom(string value);

	string getAsString(boost::shared_ptr<Atom> atom);
	double getAsDouble(boost::shared_ptr<Atom> atom);
	int getAsInt(boost::shared_ptr<Atom> atom);
	bool getAsBool(boost::shared_ptr<Atom> atom);
};

}
}
}

#endif /* INCLUDE_COMMUNICATION_ATOMCONVERTER_H_ */
