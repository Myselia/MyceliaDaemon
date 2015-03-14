/**
 * Generic definitions and classes that can be used everywhere.
 */

#ifndef INCLUDE_GENERIC_H_
#define INCLUDE_GENERIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <unordered_map>

typedef unsigned short ushort;
typedef unsigned int uint;

namespace mycelia
{

class GenericUtil
{
	static char getSecondLastDigit(int num);
	static void printScale(int len);
};

}

#endif /* INCLUDE_GENERIC_H_ */
