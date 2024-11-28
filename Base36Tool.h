#pragma once

#include <string>
#include <cmath>
#include <sstream>
#include <cctype>

/*
  Static struct created to convert IP addresses into a Base36 string.

  Assumes that IP addresses are in IPV4 format, and that they're valid.
  No error checking is present here as this tool is a simple A to B translation process.
*/
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

