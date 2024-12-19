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

    schema[columnName] = { &columnType, columnCount }; // Store type and index
    columnNames.push_back(columnName);
    columnCount++; 
}




const std::type_info& CSVMetadata::getColumnType2(const std::string& columnName) const {
    auto it = schema.find(columnName);
    if (it == schema.end()) {
        throw std::out_of_range("Column not found in metadata: " + columnName);
    }
    return *(it->second.type);
}

size_t CSVMetadata::getColumnCount() const {
    return columnCount; 
}

const std::vector<std::string>& CSVMetadata::getColumnNames() const {
    return columnNames; 
}

size_t CSVMetadata::getColumnIndex(const std::string& columnName) const {
    auto it = schema.find(columnName);
    if (it == schema.end()) {
        throw std::out_of_range("Column not found in metadata: " + columnName);
    }
    return it->second.index;
}

// This validates that a column exists and matches the expected type
void CSVMetadata::validateColumn(const std::string& columnName, const std::type_info& columnType) const {
    auto it = schema.find(columnName);
    if (it == schema.end()) {
        throw std::invalid_argument("Column not defined in metadata: " + columnName);
    }
    if (*(it->second.type) != columnType) {
        throw std::invalid_argument("Type mismatch for column: " + columnName);
    }
}

void CSVMetadata::printMetadata() const {
    for (const auto& [columnName, metadata] : schema) {
        std::string typeName;

        // Map type_info to human-readable names
        if (*metadata.type == typeid(std::string)) {
            typeName = "String";
        }
        else if (*metadata.type == typeid(int)) {
            typeName = "Integer";
        }
        else if (*metadata.type == typeid(double)) {
            typeName = "Double";
        }
        else {
            typeName = "Unknown Type";
        }

        std::cout << "Column: " << columnName << ", Type: " << typeName << ", Index: " << metadata.index << "\n";
    }
}

const std::unordered_map<std::string, ::CsvColumnMetadata>& CSVMetadata::getCSVSchema() const {
    return schema;
}



