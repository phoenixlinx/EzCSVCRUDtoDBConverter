// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef CSVMETADATA_HPP
#define CSVMETADATA_HPP

#include <map>
#include <set>
#include <string>
#include <typeindex>
#include <variant>
#include <stdexcept>
#include <iostream>

using ColumnTypes = std::variant<int, double, std::string>;
// This class was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
class CSVMetadata {
private:
    std::map<std::string, std::type_index> schema; // Stores column names and column types

    // Dynamically generate the allowed types from ColumnTypes
    static std::set<std::type_index> generateAllowedTypes();

    const std::set<std::type_index> allowedTypes;

public:
     CSVMetadata();

    // Add a column and its type to the schema
    void addColumn(const std::string& columnName, const std::type_index& columnType);

    // Get the type of a column
    std::type_index getColumnType(const std::string& columnName) const;

    //Validate that a column exists and matches the expected type
    void validateColumn(const std::string& columnName, const std::type_index& columnType) const;
    void printMetadata() const;
};

#endif
