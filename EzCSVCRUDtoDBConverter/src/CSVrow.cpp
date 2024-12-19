// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <CSVrow.hpp>





CSVrow::CSVrow(const std::string& csvPath, unsigned int rowPercentageToAnalyze)
    : id(0),// Initialize the row's unique identifier to 0 (can be updated later)

    columnCount(0) // Cache the total number of columns for efficiency
{
    // Create the analyzer and generate metadata and parser
    CSVAnalyzer analyzer(csvPath, rowPercentageToAnalyze);
    metadata = analyzer.analyzeStructure();// Save a reference to the metadata for column names and types
    parser = analyzer.getParcedCSV(); // Initialize parser
    metadata->printMetadata();
    // Initialize column count and handlers
    columnCount = metadata->getColumnCount();
    initializeHandlers();
    orderedColumnNames = metadata->getColumnNames();
    setRows(*parser); // Populate rows
}






void CSVrow::setRows(csv::Parser& parsedFile)
{
    const auto& columnNames = metadata->getColumnNames();
    size_t totalRows = parsedFile.rowCount();
    string value;
    size_t columnIndex = 0;
    // Reserve capacity for rowData to handle 160% of total rows
    reserveRowData(totalRows);


    for (size_t rowIndex = 0; rowIndex < totalRows; ++rowIndex) {

        rowData[rowIndex] = std::make_shared<std::vector<DynamicTypedValue>>(columnCount);

    }



    // Process each column individually
    for (const auto& columnName : columnNames) {
        auto handlerIt = columnHandlers.find(columnName);
        if (handlerIt == columnHandlers.end()) {
            throw std::runtime_error("Handler not found for column: " + columnName);
        }

        columnIndex = this->columnIndex[columnName];
        // Process each row for this column
        for (size_t rowIndex = 0; rowIndex < totalRows; ++rowIndex) {
            const auto& row = parsedFile.getRow(rowIndex);



            handlerIt->second(row[columnName], (*rowData[rowIndex])[columnIndex]);
        }
    }
}




size_t CSVrow::setRow(const csv::Row& parsedRow) {
    const auto& columnNames = metadata->getColumnNames();
    size_t columnIndex = 0;
    size_t mapKey = 0; // Use size_t for the map key

    // Determine the key for the new row
    mapKey = rowData.size();

    // Allocate a new shared pointer for the row and insert it into the map
    rowData[mapKey] = std::make_shared<std::vector<DynamicTypedValue>>(columnCount);

    // Populate each column in the new row
    for (const auto& columnName : columnNames) {
        auto handlerIt = columnHandlers.find(columnName);
        if (handlerIt == columnHandlers.end()) {
            throw std::runtime_error("Handler not found for column: " + columnName);
        }

        // Retrieve the column index from the column name
        columnIndex = this->columnIndex[columnName];

        // Apply the handler to populate the appropriate column in the row
        handlerIt->second(parsedRow[columnName], (*rowData[mapKey])[columnIndex]);
    }

    return mapKey;
}



void CSVrow::initializeHandlers()
{

    // Retrieve the column names from the metadata. This defines the keys for handlers and indices.
    const auto& columnNames = metadata->getColumnNames();

    // For each column, create a handler (callable object) that processes its data dynamically.
    // This eliminates repetitive conditional checks elsewhere in the codebase.
    for (const auto& columnName : columnNames) {
        // Get the type of the column (e.g., int, double, or std::string) from the metadata.
        const auto& columnType = metadata->getColumnType2(columnName);

        // Create a processing function for each column type. The function is stored in the hash table.
        if (columnType == typeid(int)) {
            // If the column type is int, store a lambda that converts the input value to an integer.
            // Use StringConverter to handle invalid or missing values gracefully (default to 0).
            columnHandlers[columnName] = [](const std::string& value, DynamicTypedValue& data) {
                data = DynamicTypedValue(StringConverter::toInt(value).value_or(0));
                };
        }
        else if (columnType == typeid(double)) {
            // If the column type is double, store a lambda that converts the input value to a double.
            // Default to 0.0 if the value cannot be converted.
            columnHandlers[columnName] = [](const std::string& value, DynamicTypedValue& data) {
                data = DynamicTypedValue(StringConverter::toDouble(value).value_or(0.0));
                };
        }
        else if (columnType == typeid(std::string)) {
            // If the column type is string, store a lambda that wraps the input value as-is.
            // No transformation or validation is needed for string types.
            columnHandlers[columnName] = [](const std::string& value, DynamicTypedValue& data) {
                data = DynamicTypedValue(value);
                };
        }
        else {
            // If the column type is unsupported, throw a runtime error.
            // This ensures the program fails early in the presence of invalid metadata.
            throw std::runtime_error("Unsupported column type in metadata.");
        }
    }

    // Create a mapping between column names and their indices in the row data vector.
    // This allows fast access to column data by name during runtime.
    for (size_t i = 0; i < columnNames.size(); ++i) {
        columnIndex[columnNames[i]] = i; // Map each column name to its index in rowData.
    }


}


void  CSVrow::setDefaultId(unsigned int id) {
    this->id = id;
}
unsigned int   CSVrow::getDefaultId() {
    return this->id;
}

const DynamicTypedValue& CSVrow::getValue(const std::vector<DynamicTypedValue>& row, const std::string& columnName) const {
    auto colIt = columnIndex.find(columnName);
    if (colIt == columnIndex.end()) {
        throw std::out_of_range("Column not found: " + columnName);
    }
    return row[colIt->second];
}



std::shared_ptr<std::vector<DynamicTypedValue>> CSVrow::getRow(size_t rowIndex) {
    auto rowDataElementPtr = rowData.find(rowIndex);
    if (rowDataElementPtr == rowData.end()) {
        throw std::out_of_range("Row index not found in rowData.");
    }
   
    return rowDataElementPtr->second;
}


void CSVrow::printAllElements() const {
    const auto& columnNames = metadata->getColumnNames();

    // Iterate through each row in the unordered_map
    for (const auto& [rowKey, rowDataPtr] : rowData) {
        if (!rowDataPtr) {
            std::cerr << "Error: Null row pointer encountered at key " << rowKey << "\n";
            continue;
        }

        // Print the row key for clarity
        std::cout << "Row Key " << rowKey << ": ";

        // Iterate through the columns for this row
        for (size_t columnIdx = 0; columnIdx < columnNames.size(); ++columnIdx) {
            const std::string& columnName = columnNames[columnIdx];
            const auto& columnValue = (*rowDataPtr)[columnIdx];

            // Print the column name and its value
            std::cout << columnName << ": " << columnValue << "\t";
        }
        std::cout << "\n"; // New line after each row
    }
}

void CSVrow::printRow(const std::vector<DynamicTypedValue>& row) {
    const auto& columnNames = metadata->getColumnNames();
    for (const auto& columnName : columnNames) {
        std::cout << columnName << ": " << getValue(row, columnName) << "\n";
    }
}

size_t CSVrow::getColumnIndex(const std::string& columnName) {
    auto it = columnIndex.find(columnName);
    if (it == columnIndex.end()) {
        throw std::out_of_range("Column not found: " + columnName);
    }
    return it->second;
}




const std::unordered_map<std::string, CsvColumnMetadata>& CSVrow::getCSVSchema() const {
    return metadata->getCSVSchema();
}




std::shared_ptr<CSVMetadata> CSVrow::getMetadata()
{
    return metadata;
}

void CSVrow::reserveRowData(size_t rowCount) {
    size_t newCapacity = rowCount * 1.6;
    rowData.reserve(newCapacity);
}

std::unordered_map<size_t, std::shared_ptr<std::vector<DynamicTypedValue>>>& CSVrow::getRowData() {
    return rowData;
}

const std::unordered_map<std::string, size_t>& CSVrow::getColumnIndexMap() const
{
    return columnIndex;
}




const vector<string>& CSVrow::getOrderedColumnNames() const
{
    return orderedColumnNames;
}



