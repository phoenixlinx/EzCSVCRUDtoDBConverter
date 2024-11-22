
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

//============================================================================
// Name        : EzCSVCRUDtoDBConverter.cpp
// Author      : Alfredo A. Martinez
// Version     : 1.7
//============================================================================
// EzCSVCRUDtoDBConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
# include <string>
# include <vector>

#include "../include/StringConverter.hpp"
#include "../include/PerformanceUtils.hpp"
#include "../include/MenuUtils.hpp"



int main(int argc, char* argv[]) {
    BinarySearchTree* binaryTreeObj = new BinarySearchTree;
    Bid bid;
    string csvPath;
    string csvPathDeletedBids;
    optional<int> choice;
    string userInput;
    switch (argc) {
    case 2:
        csvPath = argv[projectConstants::CSV_FILE_PATH];
        break;
    default:
        csvPath = "data\\eOfferMonthlySalesNov.csv";
        csvPathDeletedBids = "data\\eOfferDeletedBids.csv";


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
                    if (handleFileSelection(csvPath, binaryTreeObj)) {

                        handleCRUD(csvPath, binaryTreeObj);

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