#ifndef _ARGUMENTS_H_
#define _ARGUMENTS_H_

#include <string>

using std::string;

namespace mlib {

class Arguments
{
public:
	Arguments(int, char**);
	virtual ~Arguments();
	bool HasArgs (string) const;
	string GetArgsAsString (string) const;
	long GetArgsAsLong (string) const;
	unsigned long GetArgsAsUnsigned (string) const;
	double GetArgsAsDouble (string) const;
	int GetArgsCount () const;
protected:
	int argCount;
	string * args;
};

}

#endif //_ARGUMENTS_H_
