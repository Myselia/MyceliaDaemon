#ifndef INCLUDE_EXCEPTION_H_
#define INCLUDE_EXCEPTION_H_

#include <exception>

#include "Generic.h"

using namespace std;

namespace com
{
namespace myselia
{
namespace cpp
{

class GenericException: public runtime_error
{
	public:
	GenericException(): runtime_error("")
	{
		message="";
	}

	GenericException(string message): runtime_error(message)
	{
		this->message=message;
	}

	GenericException(exception cause): runtime_error("")
	{
		message="";
		this->cause=cause;
	}

	GenericException(exception cause, string message): runtime_error(message)
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

	const char* what() const throw()
	{
		return message.c_str();
	}

	private:
	string message;
	exception cause;
};

class IllegalStateException: public GenericException
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

class IllegalArgumentException: public GenericException
{
	public:
	IllegalArgumentException()
	{
		//Do nothing
	}

	IllegalArgumentException(string message): GenericException(message)
	{
		//Do nothing
	}

	IllegalArgumentException(exception cause): GenericException(cause)
	{
		//Do nothing
	}
};

}
}
}

#endif /* INCLUDE_EXCEPTION_H_ */
