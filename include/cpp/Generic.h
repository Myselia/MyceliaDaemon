/**
 * Generic definitions and classes that can be used everywhere.
 */

#ifndef INCLUDE_GENERIC_H_
#define INCLUDE_GENERIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

#include <boost/shared_ptr.hpp>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

namespace com
{
namespace myselia
{
namespace cpp
{

class GenericUtil
{
	static char getSecondLastDigit(int num);
	static void printScale(int len);
};

}
}
}

#endif /* INCLUDE_GENERIC_H_ */
