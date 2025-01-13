
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
int main(int argc, char* argv[]) {
  




    string csvPath;
    string csvRowIDColumn;
    optional<int> choice;
    string userInput;
    size_t idColumnIndex;
    switch (argc) {
    case 2:
        csvPath = argv[projectConstants::CSV_FILE_PATH];
        csvRowIDColumn = argv[projectConstants::CSV_ROW_ID_COLUMN];
        break;
    default:
       
        csvRowIDColumn = "Initial_Cost_KW";
    }


    while (!choice || choice.value() != projectConstants::EXIT_APPLICATION) {
        displayInitialMenu();
        cout << "Enter choice: ";
        getline(cin, userInput);

        try {
         
            if (choice = StringConverter::toInt(userInput); choice.has_value()) {
                cout << endl;

                switch (choice.value()) {
                case 1:
                    if (csvPath.empty()) {
                        
                        csvPath = handleFileSelection();
                        CSVrow csvRow(csvPath, 20);
                        idColumnIndex = csvRow.getColumnIndex(csvRowIDColumn);
                        // Define a key extractor for CSVrow
                        auto keyExtractor = [idColumnIndex](const std::vector<DynamicTypedValue>& row) {
                            return row[idColumnIndex]; // No explicit type; auto-deduces via `operator T()`
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


                        handleCRUD(csvPath, dynamicTypedValueTree,csvRow, csvRowIDColumn);
                        choice = projectConstants::EXIT_APPLICATION;
                    }
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