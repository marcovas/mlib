#include "Arguments.h"
#include <cstdlib>

namespace mlib {

Arguments::Arguments(int c, char ** v): argCount (c)
{
	args = new string [argCount];
	for (int i = 0; i < c; i++)
		args [i] = v [i];
}

int Arguments::GetArgsCount () const {
	return argCount;
}

bool Arguments::HasArgs (string arg) const {
	for (int i = 0; i < argCount; i++)
		if (arg == args [i])
			return true;
	return false;
}

string Arguments::GetArgsAsString (string arg) const {
	if (!HasArgs (arg))
		return "";
	for (int i = 0; i < argCount; i++) {
		if (arg == args [i]) {
			if (i <  (argCount - 1))
				return args [i + 1];
			else
			   return "";
		}
	}
	return "";
}

long Arguments::GetArgsAsLong (string arg) const {
	if (!HasArgs (arg))
		return 0;
	return atol (GetArgsAsString (arg).c_str());
}

unsigned long Arguments::GetArgsAsUnsigned (string arg) const {
	return (unsigned long) GetArgsAsLong (arg);
}

double Arguments::GetArgsAsDouble (string arg) const {
	if (!HasArgs (arg))
		return 0;
	return atof (GetArgsAsString (arg).c_str());
}

Arguments::~Arguments()
{
   //delete args;
}

}
