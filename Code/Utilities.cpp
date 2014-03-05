#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "Utilities.h"

void printDebugText(string msg, ...)
{
#ifdef DEBUG
	va_list args;
	va_start(args, msg);
	vprintf(msg.c_str(), args);
	va_end(args);
#endif
}

void error(string msg, ...)
{
	va_list args;
	va_start(args, msg);
	vprintf(msg.c_str(), args);
	va_end(args);

	exit(1);
}