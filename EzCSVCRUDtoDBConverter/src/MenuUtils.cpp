
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/MenuUtils.hpp"



void displayInitialMenu() {
    cout << "Menu:\n"
        " 1. Load CSV File\n"
        " 9. Exit\n";
}


void displayPrimaryMenu() {
        cout << "Menu:\n"
        " 1. Select CSV File\n"
        " 2. Display All Bids\n"
        " 3. Find Bid\n"
        " 4. Remove Bid\n"
        " 5. Insert Bid\n"
        " 9. Exit\n";
}
bool handleCRUD( string& csvPath, BinarySearchTree* binaryTreeObj) {
    Bid bid;
    //string csvPath;
    optional<int> choice;
    string userInput;
    while (!choice || choice.value() != projectConstants::EXIT_APPLICATION) {
        displayPrimaryMenu();
        cout << "Enter choice: ";
        getline(cin, userInput);
        try {

            if (choice = StringConverter::toInt(userInput); choice.has_value()) {
                cout << endl;

                switch (choice.value()) {
                case 1:
                    handleFileSelection(csvPath, binaryTreeObj);
                    break;
                case 2:
                    measurePerformance("Displaying All Bids", [&]() {binaryTreeObj->printInOrder(); });
                    break;
                case 3:
                    cout << "Enter BidId: ";
                    getline(cin, userInput);
                    measurePerformance("Finding Bid", [&]() {binaryTreeObj->search(binaryTreeObj->getRoot(), userInput); });
                    break;
                case 4:
                    cout << "Enter BidId: ";
                    getline(cin, userInput);
                    measurePerformance("Removing Bid", [&]() {binaryTreeObj->remove(userInput, csvPath, "data\\eOfferDeletedBids.csv"); });
                    break;
                case 5:
                    bid = binaryTreeObj->getBid(csvPath);
                    measurePerformance("Inserting Bid", [&]() {binaryTreeObj->insert(bid); });
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

    delete binaryTreeObj;
    return true;
}

bool handleFileSelection(string& filePath, BinarySearchTree* binarySearchTree) {
        
        cout << string(" 1. Load Previous File: ") + filePath + "\n"
        " 2. Load Bids From eOfferMonthlySalesNov.CSV\n"
        " 3. Load Bids From eOfferSales.csv\n"
        " 4. Enter New CSV File Path\n"
        " 5. Enter New CSV File Path(GUI)\n"
        " 6. Back to Main Menu\n"
        "Enter choice: ";


    string userInput;
    getline(cin, userInput);
    optional<int> choice;
    if (choice = StringConverter::toInt(userInput); choice.has_value()) {
        switch (choice.value()) {
        case 1:
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return binarySearchTree->loadBids(filePath); });
            break;
        case 2:
            filePath = "data\\eOfferMonthlySalesNov.csv";
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return binarySearchTree->loadBids(filePath); });
            break;
        case 3:
            filePath = "data\\eOfferSales.csv";
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return binarySearchTree->loadBids(filePath); });
            break;
        case 4:
            cout << "Enter the absolute CSV File Path: ";
            getline(cin, filePath);
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return binarySearchTree->loadBids(filePath); });
            break;
        case 5:
            try {
            auto filePath = csv::Parser::selectFile();
            if (filePath) {
                std::cout << "Selected file: " << filePath->string() << std::endl;
                return measurePerformance<bool>("Loading Bids", [&]() -> bool {return binarySearchTree->loadBids(filePath->string()); });
            }
            else {
                std::cout << "No file selected." << std::endl;
            }
        }
        catch (const csv::Error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        break;
        default:
            break;
        }
    }
    return true;
}// This function was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
