#pragma once

#include <string>
#include <cmath>
#include <sstream>
#include <cctype>

struct Base36Tool
{
	//Convert an integer to an IP address
	static std::string IntToIP(unsigned int _num);

	//Convert IP address to a 32-bit integer
	static unsigned int IPToInt(const std::string& _ip);

	//Encode an integer to base36
	static std::string IntToBase36(unsigned int _num);

	//Decode a base36 string to an integer
	static unsigned int Base36ToInt(const std::string& _base36Str);
};

