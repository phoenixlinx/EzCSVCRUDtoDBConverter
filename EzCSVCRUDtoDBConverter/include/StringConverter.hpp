// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef STRINGCONVERTER_HPP
#define STRINGCONVERTER_HPP

#include <string>
#include <optional>
#include <sstream>
#include <locale>
#include <unordered_set>
#include <limits>
#include <type_traits>
#include <cmath>
#include <stdexcept>
#include "projectConstants.hpp"
using namespace std;
// This class was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
class StringConverter {
public:
    // Template declaration
    template <typename NumericType>
    static bool validateStream(const std::string& inputString, const std::locale& locale = std::locale::classic());

    static inline bool isValidInt(const std::string& intString, const std::locale& locale = std::locale::classic());
    static inline bool isValidFloat(const std::string& floatString, const std::locale& locale = std::locale::classic());
    static inline bool isValidDouble(const std::string& doubleString, const std::locale& locale = std::locale::classic());
    static inline bool isValidBool(const std::string& boolString);

    static std::optional<int> toInt(const std::string& intString, const std::locale& locale = std::locale::classic());
    static std::optional<float> toFloat(const std::string& floatString, const std::locale& locale = std::locale::classic());
    static std::optional<double> toDouble(const std::string& doubleString, const std::locale& locale = std::locale::classic());

    static std::optional<bool> toBool(const std::string& boolString);
};

#include "../src/StringConverter.tpp"

#endif
