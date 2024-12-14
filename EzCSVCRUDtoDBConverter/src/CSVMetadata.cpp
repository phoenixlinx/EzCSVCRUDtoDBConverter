// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/CSVMetadata.hpp"


CSVMetadata::CSVMetadata() 
    : allowedTypes(generateAllowedTypes()), columnCount(0) {}

std::set<const std::type_info*> CSVMetadata::generateAllowedTypes() {
    return {
        &typeid(int),
        &typeid(double),
        &typeid(std::string)
    };
}

void CSVMetadata::addColumn(const std::string& columnName, const std::type_info& columnType) {
    if (allowedTypes.find(&columnType) == allowedTypes.end()) {
        throw std::invalid_argument("Unsupported type for column: " + columnName);
    }
    if (schema.find(columnName) != schema.end()) {
        throw std::invalid_argument("Duplicate column name: " + columnName);
    }
    schema[columnName] = &columnType; // Store a pointer to the type info
    columnNames.push_back(columnName);
    columnCount = columnNames.size(); // Update cached column count
}


const std::type_info& CSVMetadata::getColumnType(const std::string& columnName) const {
    auto it = schema.find(columnName);
    if (it == schema.end()) {
        throw std::out_of_range("Column not found in metadata: " + columnName);
    }
    return *(it->second); // Dereference the type info pointer
}


size_t CSVMetadata::getColumnCount() const {
    return columnCount; 
}

const std::vector<std::string>& CSVMetadata::getColumnNames() const {
    return columnNames; 
}

// This validates that a column exists and matches the expected type
void CSVMetadata::validateColumn(const std::string& columnName, const std::type_info& columnType) const {
    auto it = schema.find(columnName);
    if (it == schema.end()) {
        throw std::invalid_argument("Column not defined in metadata: " + columnName);
    }
    if (*(it->second) != columnType) {
        throw std::invalid_argument("Type mismatch for column: " + columnName);
    }
}



void CSVMetadata::printMetadata() const {
    for (const auto& [columnName, columnType] : schema) {
        std::string typeName;

        // Map type_info to human-readable names
        if (*columnType == typeid(std::string)) {
            typeName = "String";
        }
        else if (*columnType == typeid(int)) {
            typeName = "Integer";
        }
        else if (*columnType == typeid(double)) {
            typeName = "Double";
        }
        else {
            typeName = "Unknown Type";
        }

        std::cout << columnName << ": " << typeName << "\n";
    }
}

const std::unordered_map<std::string, const std::type_info*>& CSVMetadata::getCSVSchema() const
{
    return schema;
}



