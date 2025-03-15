#ifndef WINFIX_H
#define WINFIX_H

#ifdef _WIN32
#include <Windows.h>
#define WINFIX(code) code
#else
#define WINFIX(code)
#endif

#endif
