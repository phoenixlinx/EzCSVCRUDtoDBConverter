
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

//============================================================================
// Name        : EzCSVCRUDtoDBConverter.cpp
// Author      : Alfredo A. Martinez
// Version     : 2.0
//============================================================================
// EzCSVCRUDtoDBConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
# include <string>
# include <vector>

#include <StringConverter.hpp>
#include <PerformanceUtils.hpp>
#include <MenuUtils.hpp>
#include <CSVMetadata.hpp>
namespace EzCSCCRUDtoDBConverter {
    string getCSVRowIdColumn(const CSVrow& csvRow) {
        const auto& columnNames = csvRow.getOrderedColumnNames();
        string userInput;

        size_t i = 0;
        for (const auto& columnName : columnNames) {

            std::cout << i << " Column Name: " << columnName << endl;
            ++i;

        }
        std::cout << "Enter integer to select column:";
        std::getline(std::cin, userInput);
        int columnIndex = *StringConverter::toInt(userInput);

        return  columnNames.at(columnIndex);
    }

    void handle(const string& csvPath, const string& csvRowIDColumn) {





        CSVrow csvRow(csvPath, 20);

        string rowIDColumn;




        //  This if statement ensures that the user's supplied column is a CSV column.
        if (auto idColumnIndex = csvRow.getColumnIndexFromUserInput(csvRowIDColumn)) {
            // Define a key extractor for CSVrow
            auto keyExtractor = [idColumnIndex](const std::vector<DynamicTypedValue>& row) {
                return row[*idColumnIndex]; // No explicit type; auto-deduces via `operator T()`
                };



            // Create the Binary Search Tree for rows
            BinarySearchTree<std::vector<DynamicTypedValue>, decltype(keyExtractor)> dynamicTypedValueTree(keyExtractor);
            auto& allRows = csvRow.getRowData();
            for (auto& [key, rowPtr] : allRows) {
                if (rowPtr) {
                    dynamicTypedValueTree.insert(rowPtr); // Modify via the tree
                }
                else {
                    rowPtr = std::make_shared<std::vector<DynamicTypedValue>>(); // Add new row
                }
            }



            cout << "CSV rows successfully loaded into the BinarySearchTree." << endl;
            dynamicTypedValueTree.setLeftMidpoint();
            dynamicTypedValueTree.setRightMidpoint();

            handleCRUD(csvPath, dynamicTypedValueTree, csvRow, csvRowIDColumn);
        }
        else {
            std::cerr << "\nError: Column name " << csvRowIDColumn << " is  not a CSV column.\n";
            auto rowIDColumn = getCSVRowIdColumn(csvRow);
            auto idColumnIndex2 = csvRow.getColumnIndexFromUserInput(rowIDColumn);
            // Define a key extractor for CSVrow
            auto keyExtractor = [idColumnIndex2](const std::vector<DynamicTypedValue>& row) {
                return row[*idColumnIndex2]; // No explicit type; auto-deduces via `operator T()`
                };



            // Create the Binary Search Tree for rows
            BinarySearchTree<std::vector<DynamicTypedValue>, decltype(keyExtractor)> dynamicTypedValueTree(keyExtractor);
            auto& allRows = csvRow.getRowData();
            for (auto& [key, rowPtr] : allRows) {
                if (rowPtr) {
                    dynamicTypedValueTree.insert(rowPtr); // Modify via the tree
                }
                else {
                    rowPtr = std::make_shared<std::vector<DynamicTypedValue>>(); // Add new row
                }
            }



            cout << "CSV rows successfully loaded into the BinarySearchTree." << endl;
            dynamicTypedValueTree.setLeftMidpoint();
            dynamicTypedValueTree.setRightMidpoint();

            handleCRUD(csvPath, dynamicTypedValueTree, csvRow, rowIDColumn);



        }


    }
}
    int main(int argc, char* argv[]) {





        optional<string> csvPath;
        string csvRowIDColumn;
        optional<int> choice;
        string userInput;


        if (argc == 3) {
            csvPath = argv[projectConstants::CSV_FILE_PATH];
            csvRowIDColumn = argv[projectConstants::CSV_ROW_ID_COLUMN];

            if (!(csvPath->empty())) {
                EzCSCCRUDtoDBConverter::handle(*csvPath, csvRowIDColumn);
                choice = projectConstants::EXIT_APPLICATION;
            }


        }


        while (!choice || *choice != projectConstants::EXIT_APPLICATION) {
            EzCSCCRUDtoDBConverter::displayInitialMenu();
            cout << "Enter choice: ";
            getline(cin, userInput);

            try {

                if (choice = EzCSCCRUDtoDBConverter::StringConverter::toInt(userInput); choice.has_value()) {
                    cout << endl;

                    switch (*choice) {
                    case 1:
                        if (csvPath->empty()) {
                            csvPath = EzCSCCRUDtoDBConverter::handleFileSelection();
                            if (csvPath) {
                                EzCSCCRUDtoDBConverter::handle(*csvPath, csvRowIDColumn);
                            }
                        }

                        choice = projectConstants::EXIT_APPLICATION;

                        break;
                    case 9:
                        cout << "Goodbye." << endl;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                    }
                }
                else {
                    cout << userInput << " is not a valid choice." << endl;
                }
            }
            catch (const invalid_argument& ia) {
                cerr << "Invalid argument: " << ia.what() << endl;
            }
        }


        return 0;

    }
