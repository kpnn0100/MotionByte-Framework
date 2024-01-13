#pragma once
//#define DEBUG
#define RELEASE
#include <string>
inline void debug(std::string logstring)
{
#ifdef DEBUG
	std::cout << logstring << std::endl;
#endif
}

inline void debug(double number)
{
#ifdef DEBUG
	std::cout << number << std::endl;
#endif
}