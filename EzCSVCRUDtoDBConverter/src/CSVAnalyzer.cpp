// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <cmath>
#include <random>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <CSVAnalyzer.hpp>
#undef max
#undef min
namespace EzCSCCRUDtoDBConverter {

    // Infer the type of a row column
    const std::type_info* CSVAnalyzer::inferType(const std::string& cellValue) const {
        if (StringConverter::isValidInt(cellValue)) {
            return &typeid(int);
        }
        if (StringConverter::isValidDouble(cellValue)) {
            return &typeid(double);
        }
        return &typeid(std::string);
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



    void CSVAnalyzer::validateColumn(const std::string& columnName, size_t colIndex, const std::unordered_set<unsigned int>& indices, csv::Parser& file) {
        // Infer initial type from the first value
        auto firstIndex = *indices.begin();
        const std::string& firstValue = file[firstIndex][colIndex];
        const std::type_info* columnType = inferType(firstValue);
        // Begin validation based on the inferred type
        if (*columnType == typeid(int)) {
            columnType = validateAsInt(colIndex, indices, file);
        }
        else if (*columnType == typeid(double)) {
            columnType = validateAsDouble(colIndex, indices, file);
        }
        // Add the finalized type to metadata
        metadata->addColumn(columnName, *columnType);
    }

    const std::type_info* CSVAnalyzer::validateAsInt(size_t colIndex, const std::unordered_set<unsigned int>& indices, csv::Parser& file) {
        for (auto it = ++indices.begin(); it != indices.end(); ++it) {
            const std::string& cellValue = file[*it][colIndex];

            if (!StringConverter::isValidInt(cellValue)) {
                if (StringConverter::isValidDouble(cellValue)) {
                    return validateAsDouble(colIndex, indices, file); //Promote to double and continue checking for double validation
                }
                else {
                    return &typeid(std::string); // Demotes to string and stop checking more rows
                }
            }
        }
        return &typeid(int); // Remains int if no promotion
    }


    const std::type_info* CSVAnalyzer::validateAsDouble(size_t colIndex, const std::unordered_set<unsigned int>& indices, csv::Parser& file) {
        for (auto it = ++indices.begin(); it != indices.end(); ++it) {
            const std::string& cellValue = file[*it][colIndex];

            if (!StringConverter::isValidDouble(cellValue)) {
                return &typeid(std::string);
            }
        }
        return &typeid(double); // Remains double if no promotion
    }





    CSVAnalyzer::CSVAnalyzer(const std::string& csvPath, unsigned int rowPercentageToAnalyze)
        : percentToAnalyze(rowPercentageToAnalyze),
        parser(std::make_shared<csv::Parser>(csvPath)),
        metadata(std::make_shared<CSVMetadata>())
    {
    }




    std::shared_ptr<CSVMetadata> CSVAnalyzer::analyzeStructure() {

        if (!parser) {
            throw std::runtime_error("Parser is not initialized.");
        }
        if (parser->rowCount() == 0) {
            throw std::runtime_error("CSV file is empty.");
        }

        std::vector<std::string> header = parser->getHeader();
        unsigned int totalRows = parser->rowCount();
        unsigned int rowsToAnalyze = determineRowsToAnalyze(totalRows);
        // Generate random indices for row sampling
        std::unordered_set<unsigned int> indices = generateRandomRowIndices(totalRows, rowsToAnalyze);

        // Validate each column independently
        for (size_t colIndex = 0; colIndex < header.size(); colIndex++) {
            validateColumn(header[colIndex], colIndex, indices, *parser);
        }

        return metadata;
    }


    void CSVAnalyzer::printMetadata() const {
        metadata->printMetadata();
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
    std::shared_ptr<csv::Parser> CSVAnalyzer::getParcedCSV() {
        return parser;
    }
}