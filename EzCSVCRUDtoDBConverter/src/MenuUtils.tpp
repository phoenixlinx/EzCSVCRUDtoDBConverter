// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/MenuUtils.hpp"

template <typename BinarySearchTree>
bool handleCRUD(string& csvPath, BinarySearchTree& binarySearchTree, CSVrow& csvRow) {

    BidManager<BinarySearchTree> bidManager(binarySearchTree);
    std::shared_ptr<std::vector<DynamicTypedValue>> row;
    vector<shared_ptr<vector<DynamicTypedValue>>> v;
    shared_ptr<vector<DynamicTypedValue>> a;
    DynamicTypedValue b;
    DynamicTypedValue c;
    optional<int> choice;
    string userInput;
   // csvRow.getOrderedColumnNames;
    SQLiteHandler dbHandler;
    while (!choice || choice.value() != projectConstants::EXIT_APPLICATION) {
        displayPrimaryMenu();
        cout << "Enter choice: ";
        getline(cin, userInput);
        try {

            if (choice = StringConverter::toInt(userInput); choice.has_value()) {
                cout << endl;

                switch (choice.value()) {
                case 1:
                    measurePerformance("Displaying All Rows", [&]() {binarySearchTree.printInOrder(); });
                    break;
                case 2:
                    cout << "Enter Row Key: ";
                    getline(cin, userInput);
                 //   measurePerformance("Finding Rows", [&]() {binarySearchTree.search(StringConverter::toInt(userInput).value()); });
                  //  v = measurePerformance<vector<shared_ptr<vector<DynamicTypedValue>>>>("Finding Rows", [&]() {binarySearchTree.search(StringConverter::toInt(userInput).value()); });
                    v = binarySearchTree.search(StringConverter::toInt(userInput).value());
                  
                 //    a = v.at(0);
                   //  b = a->at(0);
                  //   c = a->at(1);
                    
                    break;
                case 3:
                    cout << "Enter Row Key: ";
                    getline(cin, userInput);
                    measurePerformance("Removing Row", [&]() {binarySearchTree.remove(StringConverter::toInt(userInput).value(), "data\\eOfferDeletedBids.csv"); });
                    break;
                case 4:
                    row = bidManager.getBid(binarySearchTree, csvRow);
                    measurePerformance("Inserting Row", [&]() {binarySearchTree.insert(row); });
                    break;
                case 5:



                    if (!dbHandler.openDatabase("NVT.db")) {
                        std::cerr << "Failed to open database." << std::endl;
                        return 1;
                    }

                    // Create the table with a primary key
                    try {
                        dbHandler.createTable("Bads", csvRow.getOrderedColumnNames(), csvRow.getCSVSchema(), "Initial_Cost_KW");
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error creating table: " << e.what() << std::endl;
                        return 1;
                    }


                    try {
                        dbHandler.insertRows("Bads", csvRow.getRowData());
                        std::cout << "Rows inserted successfully." << std::endl;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error inserting rows: " << e.what() << std::endl;
                    }

                    // Close the database
                    dbHandler.closeDatabase();
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


    return true;
}// This function was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)


