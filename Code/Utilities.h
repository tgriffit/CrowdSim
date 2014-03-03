#pragma once


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