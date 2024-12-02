// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#ifndef STRINGCONVERTER_TPP
#define STRINGCONVERTER_TPP



template <typename NumericType>
bool StringConverter::validateStream(const std::string& inputString, const std::locale& locale) {
    std::istringstream inputStream(inputString);
    inputStream.imbue(locale);
    NumericType value;
    inputStream >> value;

    if (inputStream.fail() || !inputStream.eof()) {
        return false;
    }

    if constexpr (std::is_integral_v<NumericType>) {
        if (value < (std::numeric_limits<NumericType>::min)() ||
            value >(std::numeric_limits<NumericType>::max)()) {
            return false;
        }
    }
    else if constexpr (std::is_floating_point_v<NumericType>) {
        if (std::isinf(value) || std::isnan(value)) {
            return false;
        }
    }

    return true;
}

#endif
