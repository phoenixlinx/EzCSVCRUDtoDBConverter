// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef CSVROW_HPP
#define CSVROW_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <functional>
#include <DynamicTypedValue.hpp>
#include <CSVMetadata.hpp>
#include <CsvColumnMetadata.hpp>
#include <CSVparser.hpp>
#include<StringConverter.hpp>
#include<CSVAnalyzer.hpp>
#include <map>

namespace EzCSCCRUDtoDBConverter {
    // This class was developed with assistance from ChatGPT 4o. See README.md.
    // Reference: OpenAI (2024)
    class CSVrow {
    private:
        std::shared_ptr<CSVMetadata> metadata;
        size_t columnCount;
        std::unordered_map<size_t, std::shared_ptr<std::vector<DynamicTypedValue>>> rowData;
        std::unordered_map<std::string, size_t> columnIndex; // Map column names to indices
        std::unordered_map<std::string, std::function<void(const std::string&, DynamicTypedValue&)>> columnHandlers;
        unsigned int id;
        std::vector<std::string> orderedColumnNames;
        std::shared_ptr<csv::Parser> parser;
        void reserveRowData(size_t rowCount);
        inline void setColumnIndex();
        void initializeHandlers();
        void setRows(csv::Parser& parsedFile);
    public:
        explicit CSVrow(const std::string& csvPath, unsigned int rowPercentageToAnalyze);
        size_t setRow(const csv::Row& parsedRow);
        // Set row data using initializer list
        void setDefaultId(unsigned int id);
        // Set row data using variadic arguments (template declaration)
        template <typename... Args>
        size_t setRowData(Args&&... args);
        template<typename InputIterator>
        size_t setRowData(InputIterator begin, InputIterator end);
        unsigned int getDefaultId();
        // Get value by column name
        const DynamicTypedValue& getValue(const std::vector<DynamicTypedValue>& row, const std::string& columnName) const;
        std::shared_ptr<std::vector<DynamicTypedValue>> getRow(size_t rowIndex);
        void printAllElements() const;
        // Print row data
        void printRow(const std::vector<DynamicTypedValue>& row);
        size_t getColumnIndex(const std::string& columnName);
        std::shared_ptr<CSVMetadata> getMetadata();
        std::optional<size_t> getColumnIndexFromUserInput(const std::string& columnName);
        const std::unordered_map < std::string, CsvColumnMetadata>& getCSVSchema() const;
        std::unordered_map<size_t, std::shared_ptr<std::vector<DynamicTypedValue>>>& getRowData();
        const std::unordered_map<std::string, size_t>& getColumnIndexMap() const;

        const std::vector<std::string>& getOrderedColumnNames() const;
    };
}

#include "../src/CSVrow.tpp" 
#endif 

