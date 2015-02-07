#ifndef INCLUDE_EXCEPTION_H_
#define INCLUDE_EXCEPTION_H_

#include <exception>

#include "Generic.h"

using namespace std;

class GenericException: exception
{
	public:
	GenericException()
	{
		message="";
		cause=NULL;
	}

	GenericException(string message)
	{
		this->message=message;
		cause=NULL;
	}

	GenericException(exception cause)
	{
		message="";
		this->cause=cause;
	}

	GenericException(exception cause, string message)
	{
		this->message=message;
		this->cause=cause;
	}

	string getMessage()
	{
		return message;
	}

	exception getCause()
	{
		return cause;
	}

	virtual const char* what() const throw()
	{
		return message;
	}

	private:
	string message;
	exception cause;
};

class IllegalStateException: GenericException
{
	public:
	IllegalStateException()
	{
		//Do nothing
	}

	IllegalStateException(string message): GenericException(message)
	{
		//Do nothing
	}

	IllegalStateException(exception cause): GenericException(cause)
	{
		//Do nothing
	}
};

#endif /* INCLUDE_EXCEPTION_H_ */
