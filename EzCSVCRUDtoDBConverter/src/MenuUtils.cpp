
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/MenuUtils.hpp"



void displayInitialMenu() {
    cout << "Menu:\n"
        " 1. Select CSV File\n"
        " 9. Exit\n";
}


void displayPrimaryMenu() {
        cout << "Menu:\n"
        " 1. Display All Rows\n"
        " 2. Find Row\n"
        " 3. Remove Row\n"
        " 4. Insert Row\n"
        " 9. Exit\n";
}


void displayFileSelectionMenu() {
    cout << 
        " 1. Enter New CSV File Path\n"
        " 2. Enter New CSV File Path(GUI)\n"
        " 7. Back to Main Menu\n"
        "Enter choice: ";
}



string handleFileSelection() {



    displayFileSelectionMenu();
    string userInput;
    string filePath;
    getline(cin, userInput);
    optional<int> choice;
    if (choice = StringConverter::toInt(userInput); choice.has_value()) {
        switch (choice.value()) {
        case 1:
            cout << "Enter the absolute CSV File Path: ";
            getline(cin, filePath);
            break;
        case 2:
            try {
                auto csvPath = csv::Parser::selectFile();
                if (csvPath) {
                    filePath = csvPath->string();
                    std::cout << "Selected file: " << csvPath->string() << std::endl;
                   
                }
                else {
                    std::cout << "No file selected." << std::endl;
                }
            }
            catch (const exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            break;
        default:
            break;
        }
    }
    return filePath;
}