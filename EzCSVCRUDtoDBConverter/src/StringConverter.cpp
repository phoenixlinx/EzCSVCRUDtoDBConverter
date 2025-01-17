// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/StringConverter.hpp"
namespace EzCSCCRUDtoDBConverter {

    bool StringConverter::isValidInt(const std::string& intString, const std::locale& locale) {
        return validateStream<int>(intString, locale);
    }

    // Validate float strings
    bool StringConverter::isValidFloat(const std::string& floatString, const std::locale& locale) {
        return validateStream<float>(floatString, locale);
    }

    // Validate double strings
    bool StringConverter::isValidDouble(const std::string& doubleString, const std::locale& locale) {
        return validateStream<double>(doubleString, locale);
    }

    // Validate boolean strings
    bool StringConverter::isValidBool(const std::string& boolString) {
        static const std::unordered_set<std::string> validBoolValues = {
            "1", "0", "true", "false", "yes", "no", "t", "f"
        };
        return validBoolValues.count(boolString) > 0;
    }

    // Convert string to integer
    std::optional<int> StringConverter::toInt(const std::string& intString, const std::locale& locale) {
        if (isValidInt(intString, locale)) {
            std::istringstream inputStream(intString);
            inputStream.imbue(locale);
            int value;
            inputStream >> value;

            // Check for wrap-around explicitly
            if (value >= std::numeric_limits<int>::min() && value <= std::numeric_limits<int>::max()) {
                return value;
            }
        }
        return std::nullopt; // Return empty optional if validation fails
    }

    // Convert string to float
    std::optional<float> StringConverter::toFloat(const std::string& floatString, const std::locale& locale) {
        if (isValidFloat(floatString, locale)) {
            std::istringstream inputStream(floatString);
            inputStream.imbue(locale);
            float value;
            inputStream >> value;

            // Check for overflow, underflow, or special values
            if (!std::isinf(value) && !std::isnan(value)) {
                return value;
            }
        }
        return std::nullopt; // Return empty optional if validation fails
    }

    // Convert string to double
    std::optional<double> StringConverter::toDouble(const std::string& doubleString, const std::locale& locale) {
        if (isValidDouble(doubleString, locale)) {
            std::istringstream inputStream(doubleString);
            inputStream.imbue(locale);
            double value;
            inputStream >> value;

            // Check for overflow, underflow, or special values
            if (!std::isinf(value) && !std::isnan(value)) {
                return value;
            }
        }
        return std::nullopt; // Return empty optional if validation fails
    }

    // Convert string to boolean
    std::optional<bool> StringConverter::toBool(const std::string& boolString) {
        if (isValidBool(boolString)) {
            return (boolString == "1" || boolString == "true" || boolString == "yes" || boolString == "t");
        }
        return std::nullopt; // Return empty optional if validation fails
    }
}