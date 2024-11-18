#include "Base36Tool.h"

std::string Base36Tool::IntToIP(unsigned int _num)
{
    std::ostringstream ipStream;

    for (int i = 3; i >= 0; i--)
    {
        ipStream << ((_num >> (i * 8)) & 0xFF);
        if (i > 0)
        {
            ipStream << '.';
        }
    }
    return ipStream.str();
}

unsigned int Base36Tool::IPToInt(const std::string& _ip)
{
    unsigned int result{ 0 };
    int shift = 24;
    size_t pos{ 0 };
    size_t nextPos = _ip.find('.');

    int byte{ 0 };

    //Split the IPv4 address into 4 parts and convert to integer
    for (int x = 0; x < 4; x++) {
        byte = std::stoi(_ip.substr(pos, nextPos - pos));
        result |= (byte << shift);
        pos = nextPos + 1;
        nextPos = _ip.find('.', pos);
        shift -= 8;
    }

    return result;
}

std::string Base36Tool::IntToBase36(unsigned int _num)
{
    //Base36 map
    const char base36Chars[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    std::string result;

    if (_num != 0)
    {
        //Convert to base36
        while (_num > 0)
        {
            result = base36Chars[_num % 36] + result;
            _num /= 36;
        }
    }
    else
    {
        result = "0";
    }

    return result;
}

unsigned int Base36Tool::Base36ToInt(const std::string& _base36Str)
{
    unsigned int result{ 0 };

    for (char c : _base36Str)
    {
        if (std::isdigit(c))
        {
            result = result * 36 + (c - '0');
        }
        else
        {
            result = result * 36 + (std::tolower(c) - 'a' + 10);
        }
    }
    return result;
}
