// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef CSV_ANALYZER_HPP
#define CSV_ANALYZER_HPP

#include <string>
#include <unordered_set>
#include <vector>
#include <typeindex>
#include <limits>
#include <cmath>
#include <random>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "CSVMetadata.hpp"
#include "CSVparser.hpp"
#include "StringConverter.hpp"
// This class was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
class CSVAnalyzer {
private:
    static constexpr unsigned int One_Hundred_Percent = 100;
    static constexpr unsigned int Zero_Percent = 0;
    static constexpr unsigned int Rounding_Adjustment = 99;
    std::string csvPath;                   
    double percentToAnalyze;               // Percentage of rows to analyze
    CSVMetadata metadata;                  // Metadata for inferred column types
    csv::Parser parseCSV() const;        
    std::type_index inferType(const std::string& cellValue) const; // Infer the type of a cell value
    unsigned int determineRowsToAnalyze(unsigned int totalRows) const; // Determine the number of rows to analyze
    std::unordered_set<unsigned int> generateRandomRowIndices(unsigned int totalRows, unsigned int rowsToAnalyze); // Generate random row indices
    void validateColumn(const std::string& columnName, size_t colIndex, const std::unordered_set<unsigned int>& indices, csv::Parser& file); // Validate a single column
    std::type_index validateAsInt(size_t colIndex, const std::unordered_set<unsigned int>& indices, csv::Parser& file); 
    std::type_index validateAsDouble(size_t colIndex, const std::unordered_set<unsigned int>& indices, csv::Parser& file); 

public:
    CSVAnalyzer(const std::string& csvPath, double percent);
    CSVMetadata analyzeStructure();
    void printMetadata() const;
};

#endif 
