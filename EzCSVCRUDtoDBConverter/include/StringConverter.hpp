// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef STRINGPARSER_HPP
#define STRINGPARSER_HPP

#include <string>
#include <optional>
#include <limits>
#include <stdexcept>
#include "projectConstants.hpp"
using namespace std;
// This class was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
class StringConverter {
public:
    static optional<int> toInt(const string& intStringRepresentation);
    static optional<float> toFloat(const string& floatStringRepresentation);
    static optional<double> toDouble(const string& doubleStringRepresentation);
    static optional<bool> toBool(const  string& boolStringRepresentation);
};

#endif
