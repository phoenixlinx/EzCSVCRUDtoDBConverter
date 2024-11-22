// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/StringConverter.hpp"

optional<int> StringConverter::toInt(const string& intStringRepresentation) {
    
    int value;
    size_t pos;
    try {
        
     value  = stoi(intStringRepresentation, &pos);

        // This ensures the enire  string is valid
        if (pos == intStringRepresentation.length()) {
            return value;
        }
    }
    catch (const invalid_argument&) {
        
    }
    catch (const out_of_range&) {
        
    }
    return nullopt;
}

optional<float> StringConverter::toFloat(const string& floatStringRepresentation) {
    size_t pos;
    float value;
    try {

        value = stof(floatStringRepresentation, &pos);
        if (pos == floatStringRepresentation.length()) {
            return value;
        }
    }
    catch (const invalid_argument&) {
        
    }
    catch (const out_of_range&) {
        
    }
    return nullopt;
}
optional<double> StringConverter::toDouble(const string& doubleStringRepresentation) {
    size_t pos;
    double value;
    try {

        value = stod(doubleStringRepresentation, &pos);
        if (pos == doubleStringRepresentation.length()) {
            return value;
        }
    }
    catch (const invalid_argument&) {

    }
    catch (const out_of_range&) {

    }
    return nullopt;
}
optional<bool> StringConverter::toBool(const string& boolStringRepresentation) {
    if (boolStringRepresentation == "1" || boolStringRepresentation == "true") {
        return true;
    }
    else if (boolStringRepresentation == "0" || boolStringRepresentation == "false") {
        return false;
    }
    return nullopt;
}