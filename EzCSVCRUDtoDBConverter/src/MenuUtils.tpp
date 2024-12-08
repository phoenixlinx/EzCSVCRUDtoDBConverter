// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/MenuUtils.hpp"

template <typename BinarySearchTree>
bool handleCRUD(string& csvPath, BinarySearchTree& binarySearchTree, CSVrow& csvRow){

    BidManager<BinarySearchTree> bidManager(binarySearchTree);
    std::shared_ptr<std::vector<DynamicTypedValue>> row;
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
                    measurePerformance("Displaying All Bids", [&]() {binarySearchTree.printInOrder(); });
                    break;
                case 2:
                    cout << "Enter BidId: ";
                    getline(cin, userInput);
                    measurePerformance("Finding Bid", [&]() {binarySearchTree.search(StringConverter::toInt(userInput).value()); });
                    break;
                case 3:
                    cout << "Enter BidId: ";
                    getline(cin, userInput);
                    measurePerformance("Removing Bid", [&]() {binarySearchTree.remove(StringConverter::toInt(userInput).value(), "data\\eOfferDeletedBids.csv"); });
                    break;
                case 4:
                    row = bidManager.getBid(binarySearchTree,csvRow);
                    measurePerformance("Inserting Bid", [&]() {binarySearchTree.insert(row); });
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

  //  delete binarySearchTree;
    return true;
}

template <typename BinarySearchTree>
bool handleFileSelection2(string& filePath, BinarySearchTree& binarySearchTree){
   
       

    BidManager<BinarySearchTree> bidManager(binarySearchTree);
    string userInput;
    getline(cin, userInput);
    optional<int> choice;
    if (choice = StringConverter::toInt(userInput); choice.has_value()) {
        switch (choice.value()) {
        case 1:
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return bidManager.loadBids(filePath); });
            break;
        case 2:
            filePath = "data\\eOfferMonthlySalesNov.csv";
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return bidManager.loadBids(filePath); });
            break;
        case 3:
            filePath = "data\\eOfferSales.csv";
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return bidManager.loadBids(filePath); });
            break;
        case 4:
            cout << "Enter the absolute CSV File Path: ";
            getline(cin, filePath);
            return measurePerformance<bool>("Loading Bids", [&]() -> bool {return bidManager.loadBids(filePath); });
            break;
        case 5:
            try {
            auto csvPath = csv::Parser::selectFile();
            if (csvPath) {
                std::cout << "Selected file: " << csvPath->string() << std::endl;
                return measurePerformance<bool>("Loading Bids", [&]() -> bool {  return bidManager.loadBids(csvPath->string()); });
            }
            else {
                std::cout << "No file selected." << std::endl;
            }
        }
        catch (const csv::Error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        break;
        case 6:
            filePath = "data\\eOfferMonthlySalesNov.csv";
            return measurePerformance<bool>("Loading CSVrows", [&]() -> bool {return bidManager.loadCSVrows(filePath); });
            break;
        default:
            break;
        }
    }
    return true;
}// This function was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
