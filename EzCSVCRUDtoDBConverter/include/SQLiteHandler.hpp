// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef SQLITEHANDLER_HPP
#define SQLITEHANDLER_HPP

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <sqlite3.h>
#include <ranges>
#include <typeindex>
#include <DynamicTypedValue.hpp>
#include <SqliteValidator.hpp>
#include <CsvColumnMetadata.hpp>

class SQLiteHandler {
private:

    struct ColumnMetadata {
        std::string originalName;  // Original column name (from CSV/schema)
        std::string quotedName;    // Quoted column name (for SQL)
        const std::type_info* type; // Type of the column (e.g., int, string, etc.)
    };



    // Table structure to hold metadata
    struct Table {
        std::string tableName;
        std::string primaryKeyName;
        std::unordered_map<std::string, size_t> vectorColumnIndex; // Column names to indices
        std::unordered_map<std::string, const std::type_info*> vectorSchema; // Column names to C++ types
        std::vector<ColumnMetadata> metadata;
        size_t tableColumnCount;
        size_t initialTableColumnCount;

        Table();
    };

    sqlite3* db; // SQLite database handle
    std::unordered_map<std::string, Table> tableRegistry; // Registry of created tables

    void executeQuery(const std::string& query);
    void bindValues(sqlite3_stmt* stmt, const std::vector<DynamicTypedValue>& row, const Table& table);
    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();
public:
    SQLiteHandler();
    ~SQLiteHandler();

    bool openDatabase(const std::string& dbPath);
    void closeDatabase();
  
    // Overloaded createTable functions

    bool createTable(const std::string& tableName, const std::vector<std::string>& orderedColumnNames, const std::unordered_map<std::string, CsvColumnMetadata>& schema, const std::string& primaryKey);

    bool createTable(const std::string& tableName, const std::vector<std::string>& orderedColumnNames, const std::unordered_map<std::string, CsvColumnMetadata>& schema);

  
    bool insertRows(const std::string& tableName, const std::unordered_map<size_t, std::shared_ptr<std::vector<DynamicTypedValue>>>& rowData);
  

};

#endif 