#pragma once

#include "Utils.hpp"

std::string Utils::split(std::string const& str, std::string const& del)
{
    // create a stream from the string  
    short unsigned int endPart1 = str.find_first_of(del);
    std::string part1 = str.substr(0, endPart1);
    std::string part2 = str.substr(endPart1 + 1);
    return part2;
}