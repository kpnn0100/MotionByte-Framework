#pragma once
//#define DEBUG
#define RELEASE
#define FPS_SHOW true
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

inline void test_debug(std::string logstring)
{
	std::cout << logstring << std::endl;
}