#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>
#include <cstdlib>


#ifndef BVH_UTIL_H
#define BVH_UTIL_H

using namespace std;

string seekLine(ifstream&, string);
string checkNextLine(ifstream&, string);

#endif

