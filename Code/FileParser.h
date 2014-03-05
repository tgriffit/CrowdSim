#pragma once

#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>
#include <cstdlib>

#include "Utilities.h"

// Navigates to the next line that begins with the pattern.
string seekLine(ifstream& file, string pattern);

// Checks whether the next line begins with the pattern.
string checkNextLine(ifstream& file, string pattern);