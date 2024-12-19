// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef CSVMETADATA_HPP
#define CSVMETADATA_HPP

#include <unordered_map>
#include <set>
#include <string>
#include <typeindex>
#include <variant>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <CsvColumnMetadata.hpp>

// This class was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
class CSVMetadata {
private:
    std::unordered_map<std::string, CsvColumnMetadata> schema;  // Keys are column names. Values are structs storing the type and index of the column in the DynamicTypedValue vector.
 //   std::unordered_map<std::string, const std::type_info*> schema;  // Stores column names and column types
    const std::set<const std::type_info*> allowedTypes;
    // Dynamically generate the allowed types from ColumnTypes
    static std::set<const std::type_info*> generateAllowedTypes();
    std::vector<std::string> columnNames;
    size_t columnCount;

public:
     CSVMetadata();
   //  CSVMetadata(const CSVMetadata&) = delete;            // Prevent copying
   //  CSVMetadata& operator=(const CSVMetadata&) = delete; // Prevent copy assignment
     CSVMetadata(CSVMetadata&&) = default;                 // Allow moving
     CSVMetadata& operator=(CSVMetadata&&) = default;      // Allow move assignment
    // Add a column and its type to the schema
    
     void addColumn(const std::string& columnName, const std::type_info& columnType);
    // Get the type of a column
    const std::type_info& getColumnType2(const std::string& columnName) const;
    size_t getColumnCount() const;
    const std::vector<std::string>& getColumnNames() const;
    size_t getColumnIndex(const std::string& columnName) const;
    //Validate that a column exists and matches the expected type
    void validateColumn(const std::string& columnName, const std::type_info& columnType) const;
    void printMetadata() const;
    const std::unordered_map<std::string, CsvColumnMetadata>& getCSVSchema() const;
};

#endif
