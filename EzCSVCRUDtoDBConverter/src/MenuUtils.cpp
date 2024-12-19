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
        " 5. Create Table\n"
        " 9. Exit\n";
}


void displayFileSelectionMenu() {
    cout << 
        " 1. Enter New CSV File Path\n"
        " 2. Enter New CSV File Path(GUI)\n"
        " 9. Back to Main Menu\n"
        "Enter choice: ";
}



string handleFileSelection() {



 
    string userInput;
    string filePath;
    optional<int> choice;
    while (!choice || choice.value() != projectConstants::EXIT_APPLICATION) {
        displayFileSelectionMenu();
        getline(cin, userInput);
        if (choice = StringConverter::toInt(userInput); choice.has_value()) {
            switch (choice.value()) {
            case 1:
                cout << "Enter the absolute CSV File Path: ";
                getline(cin, filePath);
                choice = projectConstants::EXIT_APPLICATION;
                break;
            case 2:
                try {
                    auto csvPath = csv::Parser::selectFile();
                    if (csvPath) {
                        filePath = csvPath->string();
                        std::cout << "Selected file: " << csvPath->string() << std::endl;
                        choice = projectConstants::EXIT_APPLICATION;
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
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    }
    return filePath;
}


