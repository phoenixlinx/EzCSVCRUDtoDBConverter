// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "BidManager.hpp"

template <typename BinarySearchTree>
BidManager<BinarySearchTree>::BidManager(BinarySearchTree& bst) : bst(bst) {}

template <typename BinarySearchTree>
std::shared_ptr<std::vector<DynamicTypedValue>> BidManager<BinarySearchTree>::getBid(BinarySearchTree& tree, CSVrow& csvRow) {
    // Create a new CSVrow object for user input
    std::string input;
    bool validInput = false;
    size_t newVectorIndex = 0;
    std::vector<std::string> rowData; // Vector to hold row values

    auto metadata = csvRow.getMetadata();
    if (!metadata || metadata->getColumnNames().empty()) {
        std::cerr << "Invalid or missing metadata in CSVrow.\n";
        return nullptr;
    }

    const auto& columnNames = metadata->getColumnNames();
    rowData.reserve(columnNames.size());
    std::cout << "Enter Row Details:\n";

    for (const auto& columnName : columnNames) {
        const auto& columnType = metadata->getColumnType2(columnName);
        validInput = false;

        while (!validInput) {
            std::cout << "Enter value for " << columnName << " (";

            if (columnType == typeid(int)) {
                std::cout << "Integer";
            }
            else if (columnType == typeid(double)) {
                std::cout << "Double";
            }
            else {
                std::cout << "String";
            }

            std::cout << "): ";

            std::getline(std::cin, input);

            try {
                // Validate input type
                if (columnType == typeid(int) && !StringConverter::isValidInt(input)) {
                    throw std::invalid_argument("Invalid Integer");
                }
                else if (columnType == typeid(double) && !StringConverter::isValidDouble(input)) {
                    throw std::invalid_argument("Invalid Double");
                }

                rowData.push_back(input); // Append valid input to rowData
                validInput = true; // Exit loop on successful input
            }
            catch (const std::exception& e) {
                std::cerr << "Invalid input for column: " << columnName
                    << ". Error: " << e.what() << "\n";
            }
        }
    }

    // Use setRow to add the row
    csv::Row newRow(metadata->getColumnNames());
    for (const auto& value : rowData) {
        newRow.push(value);
    }

    newVectorIndex = csvRow.setRow(newRow); // Set the newly created row


    auto newRowVector = csvRow.getRow(newVectorIndex);

    return newRowVector; // Return the created row's vector
}
// This function was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)



