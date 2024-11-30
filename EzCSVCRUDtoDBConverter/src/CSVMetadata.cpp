// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/CSVMetadata.hpp"


CSVMetadata::CSVMetadata() 
    : allowedTypes(generateAllowedTypes()) {}

std::set<std::type_index> CSVMetadata::generateAllowedTypes() {
    return {
        std::type_index(typeid(int)),
        std::type_index(typeid(double)),
        std::type_index(typeid(std::string))
    };
}

void CSVMetadata::addColumn(const std::string& columnName, const std::type_index& columnType) {
       if (allowedTypes.find(columnType) == allowedTypes.end()) {
        throw std::invalid_argument("Unsupported type for column: " + columnName);
    }
    schema.try_emplace(columnName, columnType); // Add column if valid
}


std::type_index CSVMetadata::getColumnType(const std::string& columnName) const {
    auto it = schema.find(columnName);
    if (it == schema.end()) {
        throw std::out_of_range("Column not found in metadata: " + columnName);
    }
    return it->second;
}

// This validates that a column exists and matches the expected type
void CSVMetadata::validateColumn(const std::string& columnName, const std::type_index& columnType) const {
    auto it = schema.find(columnName);
    if (it == schema.end()) {
        throw std::invalid_argument("Column not defined in metadata: " + columnName);
    }
    if (it->second != columnType) {
        throw std::invalid_argument("Type mismatch for column: " + columnName);
    }
}



void CSVMetadata::printMetadata() const {
    for (const auto& [columnName, columnType] : schema) {
        std::string typeName;

        // Prioritize String, then handle other types
        if (columnType == typeid(std::string)) {
            typeName = "String";
        }
        else if (columnType == typeid(int)) {
            typeName = "Integer";
        }
        else if (columnType == typeid(double)) {
            typeName = "Double";
        }
        else {
            typeName = "Unknown Type";
        }

        std::cout << columnName << ": " << typeName << "\n";
    }
}
