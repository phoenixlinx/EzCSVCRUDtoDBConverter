// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <SQLiteHandler.hpp>



SQLiteHandler::SQLiteHandler() : db(nullptr) {}


SQLiteHandler::~SQLiteHandler() {
    closeDatabase();
}

SQLiteHandler::Table::Table()
    : tableColumnCount(0),initialTableColumnCount(0) {
    // Containers and strings are automatically initialized
}



bool SQLiteHandler::openDatabase(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}


void SQLiteHandler::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}


void SQLiteHandler::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + error);
    }
}


void SQLiteHandler::beginTransaction() {
    executeQuery("BEGIN TRANSACTION;");
}


void SQLiteHandler::commitTransaction() {
    executeQuery("COMMIT;");
}

// Rollback transaction
void SQLiteHandler::rollbackTransaction() {
    executeQuery("ROLLBACK;");
}

// This maps C++ types to SQLite types. Its purpose is to allow for type validation. 
std::string mapCppTypeToSQLiteType(const std::type_info* type) {
    if (*type == typeid(int)) {
        return "INTEGER";
    }
    else if (*type == typeid(double)) {
        return "REAL";
    }
    else if (*type == typeid(std::string)) {
        return "TEXT";
    }
    else {
        throw std::invalid_argument("Unsupported C++ type for SQLite: " + std::string(type->name()));
    }
}
void SQLiteHandler::bindValues(sqlite3_stmt* stmt, const std::vector<DynamicTypedValue>& row, const Table& table) {
    size_t colIdx = 1; // SQLite binding starts at 1

    for (size_t i = 0; i < table.metadata.size(); ++i) {
        const ColumnMetadata& column = table.metadata[i]; // Access metadata by index
        const DynamicTypedValue& value = row[i];         // Access row value by index
        const auto* type = column.type;                 // Get the type from metadata

        // Print column name and type information for debugging
        std::cout << "Processing column: " << column.originalName
            << ", Quoted name: " << column.quotedName
            << ", Expected type: " << type->name()
            << ", Value type: " << value.getStoredTypeName() << std::endl;

        // Bind value based on its type
        if (*type == typeid(int)) {
            sqlite3_bind_int(stmt, static_cast<int>(colIdx), value.getValue<int>());
        }
        else if (*type == typeid(double)) {
            sqlite3_bind_double(stmt, static_cast<int>(colIdx), value.getValue<double>());
        }
        else if (*type == typeid(std::string)) {
            sqlite3_bind_text(stmt, static_cast<int>(colIdx), value.getValue<std::string>().c_str(), -1, SQLITE_TRANSIENT);
        }
        else {
            throw std::runtime_error("Unsupported column type: " + std::string(type->name()));
        }
        colIdx++;
    }
}

bool SQLiteHandler::createTable(const std::string& tableName,const std::vector<std::string>& orderedColumnNames,const std::unordered_map<std::string, const std::type_info*>& schema,const std::string& primaryKey){
    std::string quotedTableName = SqliteValidator::quoteColumnName(tableName);//Sanitize table name.
    if (tableRegistry.find(quotedTableName) != tableRegistry.end()) {
        throw std::runtime_error("Table already exists: " + quotedTableName);
    }

    std::string query;
    Table table;
    table.tableColumnCount = orderedColumnNames.size();
    table.initialTableColumnCount = table.tableColumnCount;
    bool primaryKeyAdded = false;
    size_t metadataElementCount = 0;
    // Handle primary key
    std::string actualPrimaryKey = primaryKey;

    // Validate column names and quote them
    if (!SqliteValidator::isPrintableUtf8(orderedColumnNames)||!SqliteValidator::isPrintableUtf8(tableName)) {
        throw std::runtime_error("Invalid column, table, or database name: not UTF-8 printable.");
    }
    auto quotedColumnMap = SqliteValidator::quoteColumnName(orderedColumnNames);
    table.tableName = quotedTableName;
    // Process columns in order
    for (size_t index = 0; index < table.tableColumnCount; ++index) {
        const std::string& originalName = orderedColumnNames[index];
        ColumnMetadata metadata = {
            .originalName = originalName,
            .quotedName = quotedColumnMap[originalName],
            .type = schema.at(originalName)
        };

        table.metadata.push_back(metadata);
    }

    table.tableColumnCount = table.metadata.size();

    // Build the CREATE TABLE SQL statement
    std::ostringstream createStmt;
    createStmt << "CREATE TABLE " << table.tableName << " (";

    if (!primaryKey.empty()) {
        // Validate primary key
        if (schema.find(primaryKey) == schema.end()) {
            throw std::runtime_error("Primary key '" + primaryKey + "' does not exist in schema.");
        }

        const auto* pkType = schema.at(primaryKey);
        if (*pkType != typeid(int) && *pkType != typeid(std::string)) {
            throw std::runtime_error("Primary key must be INTEGER or TEXT.");
        }

        for (const auto& column : table.metadata) {
            if (column.originalName == primaryKey) {
                createStmt << column.quotedName << " "
                    << ((*pkType == typeid(int)) ? "INTEGER PRIMARY KEY AUTOINCREMENT, "
                        : "TEXT PRIMARY KEY NOT NULL, ");
                primaryKeyAdded = true;
                break;
            }
        }

        if (!primaryKeyAdded) {
            throw std::runtime_error("Primary key '" + primaryKey + "' not found in metadata.");
        }
    }
    else {
        // Generate default primary key if none provided
        actualPrimaryKey = "id";
        createStmt << "\"" << actualPrimaryKey << "\" INTEGER PRIMARY KEY AUTOINCREMENT, ";
    }

    // Add remaining columns
    for (const auto& column : table.metadata) {
        if (column.originalName == actualPrimaryKey) {
            continue; // Skip primary key
        }
        createStmt << column.quotedName << " "
            << mapCppTypeToSQLiteType(column.type) << ", ";
    }

    query = createStmt.str();
    query.pop_back(); // Remove trailing comma
    query.pop_back(); // Remove trailing space
    query += ");";

    // Execute the query
    try {
        beginTransaction();
        executeQuery(query);
        tableRegistry[table.tableName] = table;
        commitTransaction();
    }
    catch (const std::exception& e) {
        rollbackTransaction();
        throw std::runtime_error("Failed to create table: " + std::string(e.what()));
    }

    return true;
}






bool SQLiteHandler::createTable(const std::string& tableName, const std::vector<std::string>& orderedColumnNames,const std::unordered_map<std::string, const std::type_info*>& schema){
    return createTable(tableName, orderedColumnNames, schema, std::string());
}

//TODO: Handle the situation where primary key column contains duplicate values.
bool SQLiteHandler::insertRows(const std::string& tableName,const std::unordered_map<size_t, std::shared_ptr<std::vector<DynamicTypedValue>>>& rowData){
    size_t metadataElementCount = 0;
    auto tableIt = tableRegistry.find(SqliteValidator::quoteColumnName(tableName));
    if (tableIt == tableRegistry.end()) {
        throw std::runtime_error("Table does not exist: " + tableName);
    }

    const Table& table = tableIt->second;
    metadataElementCount =  table.metadata.size();
    // Build the INSERT SQL statement
    std::ostringstream insertStmt;
    insertStmt << "INSERT INTO " << tableName << " (";

   
    for (const auto& column : table.metadata) {
        insertStmt << column.quotedName << ", ";
    }

    std::string query = insertStmt.str();
    query.pop_back(); // Remove trailing comma
    query.pop_back();
    query += ") VALUES (";

    // Add placeholders for values
    for (size_t i = 0; i < metadataElementCount; ++i) {
        query += "?, ";
    }

    query.pop_back(); // Remove trailing comma
    query.pop_back();
    query += ");";

    sqlite3_stmt* stmt;


    std::cout << query;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        rollbackTransaction();
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }

    beginTransaction();

    try {
        for (const auto& [rowIndex, rowPtr] : rowData) {
            const auto& row = *rowPtr;

            if (row.size() != metadataElementCount) {
                throw std::runtime_error("Row size does not match table schema.");
            }

            // Use the existing `bindValues` function to bind values
            bindValues(stmt, row, table);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
            }

            sqlite3_reset(stmt);
        }

        commitTransaction();
    }
    catch (...) {
        rollbackTransaction();
        sqlite3_finalize(stmt);
        throw;
    }

    sqlite3_finalize(stmt);
    return true;
}
