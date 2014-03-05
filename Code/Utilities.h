#pragma once

#include <string>
#include <Eigen/Dense>

// It's incredibly bad style to put usings in a header, but I give zero fucks right now.
using namespace std;
using namespace Eigen;

// Visual Studio defines _DEBUG when compiled in debug mode. This standardizes it to use the DEBUG flag across plaforms.
#ifdef _DEBUG
#define DEBUG
#endif

// If we're not in debug, disable asserts
#ifndef DEBUG
#ifndef NDEBUG
#define NDEBUG
#endif
#endif

#ifdef USINGGCC			
#define OVERRIDE					// For some reason g++ 4.6.3 (the version on the lab machines) doesn't support the override keyword
#else
#define OVERRIDE override
#endif

#define DEG2RAD 0.017453292519943	// rad = deg*PI/180 = deg*DEG2RAD
#define RAD2DEG 57.29577951308232	// deg = rad*180/PI = rad*RAD2DEG

// Prints the given text if we're in debug mode. Ignores it if we're not.
// Uses the same syntax as printf.
void printDegugText(string msg, ...);

// Print an error message and quit.
void error(string msg, ...);