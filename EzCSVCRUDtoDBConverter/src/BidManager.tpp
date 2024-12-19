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


template <typename BinarySearchTree>
bool BidManager<BinarySearchTree>::loadBids(std::string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    bst.reSetDeepestLevel();

    try {

        // initialize the CSV Parser using the given path
        csv::Parser file = csv::Parser(csvPath);
        // read and display header row - optional
        optional <double> amount;
        vector<string> header = file.getHeader();
        for (auto const& c : header) {
            cout << c << " | ";
        }
        cout << "" << endl;


        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            // Create a data structure and add to the collection of bids
            shared_ptr<Bid> bid = make_shared<Bid>();
            bid->bidId = StringConverter::toInt(file[i][1]).value();
            bid->title = file[i][0];
            bid->fund = file[i][8];
            bid->rowPos = i;
            if (bid->amount = StringConverter::toDouble(file[i][4]).value(); amount.has_value()) {

            }



            bst.insert(bid);

        }
    }
    catch (const csv::Error& e) {
        string message = e.what();

        if (message.find("No data") != string::npos) {
            cerr << "The file is empty. Please provide a file with valid data." << endl;

        }
        else if (message.find("Failed to open") != string::npos) {
            cerr << "Unable to open the file. Check the file path or permissions." << endl;

        }
        else {
            cerr << "An unknown error occurred: " << message << endl;
        }
        return false;
    }
    cout << "Bids read: " << bst.getBSTSize() << endl;
    cout << "Deepest level: " << bst.getDeepestLevel() << endl;
    return true;
}

template <typename BinarySearchTree>
bool  BidManager<BinarySearchTree>::loadCSVrows(const string& csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    bst.reSetDeepestLevel();

    try {

        CSVrow row(csvPath, 20);
        row.printAllElements();


    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return false;
    }

    cout << "CSV rows successfully loaded into the BinarySearchTree." << endl;
    cout << "Size of tree: " << bst.getBSTSize() << endl;
    cout << "Deepest level: " << bst.getDeepestLevel() << endl;
    return true;
}

