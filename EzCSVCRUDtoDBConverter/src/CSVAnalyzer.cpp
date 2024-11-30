// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <cmath>
#include <random>
#include <iostream>
#include <stdexcept>
#include <limits>
#include "../include/CSVAnalyzer.hpp"
#undef max
#undef min
csv::Parser CSVAnalyzer::parseCSV() const {
    csv::Parser file(csvPath);
    if (file.rowCount() == 0) {
        throw std::runtime_error("CSV file is empty.");
    }
    return file;
}

// Infer the type of a single cell value
std::type_index CSVAnalyzer::inferType(const std::string& cellValue) const {
    if (StringConverter::isValidInt(cellValue)) {
        return typeid(int);
    }
    if (StringConverter::isValidDouble(cellValue)) {
        return typeid(double);
    }
    return typeid(std::string);
}




// To prevent bias generate a set of random unique indices
std::unordered_set<unsigned int> CSVAnalyzer::generateRandomRowIndices(unsigned int totalRows, unsigned int rowsToAnalyze) {
    std::unordered_set<unsigned int> indices;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dist(0, totalRows - 1);

    while (indices.size() < rowsToAnalyze) {
        indices.insert(dist(gen)); // Insert random index until we have enough
    }

    return indices;
}

void CSVAnalyzer::validateColumn(
    const std::string& columnName,
    size_t colIndex,
    const std::unordered_set<unsigned int>& indices,
    csv::Parser& file
) {
    // Infer initial type from the first value
    auto firstIndex = *indices.begin();
    const std::string& firstValue = file[firstIndex][colIndex];
    std::type_index columnType = inferType(firstValue);

    // Begin validation based on the inferred type
    if (columnType == typeid(int)) {
        columnType = validateAsInt(colIndex, indices, file);
    }
    else if (columnType == typeid(double)) {
        columnType = validateAsDouble(colIndex, indices, file);
    }

    // Add the finalized type to metadata (ONE CALL ONLY)
    metadata.addColumn(columnName, columnType);
}

std::type_index CSVAnalyzer::validateAsInt(
    size_t colIndex,
    const std::unordered_set<unsigned int>& indices,
    csv::Parser& file
) {
    std::type_index columnType = typeid(int);

    for (auto it = ++indices.begin(); it != indices.end(); ++it) {
        const std::string& cellValue = file[*it][colIndex];

        if (!StringConverter::isValidInt(cellValue)) {
            if (StringConverter::isValidDouble(cellValue)) {
                columnType = typeid(double); // Promote to double
                return validateAsDouble(colIndex, indices, file); // Continue as double
            }
            else {
                return typeid(std::string); // Promote to string and stop
            }
        }
    }
    return columnType; 
}

std::type_index CSVAnalyzer::validateAsDouble(
    size_t colIndex,
    const std::unordered_set<unsigned int>& indices,
    csv::Parser& file
) {
    for (auto it = ++indices.begin(); it != indices.end(); ++it) {
        const std::string& cellValue = file[*it][colIndex];

        if (!StringConverter::isValidDouble(cellValue)) {
            return typeid(std::string); // Promote to string and stop
        }
    }
    return typeid(double); // Remains double if no promotion
}



// Constructor
CSVAnalyzer::CSVAnalyzer(const std::string& csvPath, double percent)
    : csvPath(csvPath), percentToAnalyze(percent) {
}

// Main method to analyze the CSV structure
CSVMetadata CSVAnalyzer::analyzeStructure() {
    csv::Parser file = parseCSV();                    // Parse CSV
    std::vector<std::string> header = file.getHeader(); // Read header
    unsigned int totalRows = file.rowCount();
    unsigned int rowsToAnalyze = determineRowsToAnalyze(totalRows);

    // Generate random indices for row sampling
    std::unordered_set<unsigned int> indices = generateRandomRowIndices(totalRows, rowsToAnalyze);

    // Validate each column independently
    for (size_t colIndex = 0; colIndex < header.size(); colIndex++) {
        validateColumn(header[colIndex], colIndex, indices, file);
    }

    return metadata; // Return the final metadata
}

// Print the inferred metadata
void CSVAnalyzer::printMetadata() const {
    metadata.printMetadata();
}

unsigned int CSVAnalyzer::determineRowsToAnalyze(unsigned int totalRows) const {

    if (percentToAnalyze == Zero_Percent || percentToAnalyze > One_Hundred_Percent) {
        throw std::invalid_argument("Invalid input: Percentage to analyze must be in the range [1, 100].");
    }

    // This check for multiplication overflow
    if (totalRows > std::numeric_limits<unsigned long long>::max() / percentToAnalyze) {
        throw std::overflow_error("Overflow detected: totalRows * percentToAnalyze exceeds limits.");
    }

    // Calculate rows to analyze using safe integer arithmetic
    unsigned long long scaledRows = static_cast<unsigned long long>(totalRows) * percentToAnalyze;
    // Check if adding will overflow
    if (scaledRows > std::numeric_limits<unsigned long long>::max() - Rounding_Adjustment) {
        throw std::overflow_error("Overflow detected: scaledRows +  Rounding_Adjustment exceeds limits.");
    }

    
    unsigned long long calculatedRows = (scaledRows + Rounding_Adjustment) / One_Hundred_Percent;

    // Validate calculatedRows against unsigned int maximum
    if (calculatedRows > std::numeric_limits<unsigned int>::max()) {
        throw std::overflow_error("Overflow detected: Calculated rows exceed unsigned int maximum.");
    }

  
    unsigned int rowsToAnalyze = std::min(static_cast<unsigned int>(calculatedRows), totalRows);

    return rowsToAnalyze;
}
