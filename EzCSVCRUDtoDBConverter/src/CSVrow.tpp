// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef CSVROW_TPP
#define CSVROW_TPP
#include<CSVrow.hpp>


namespace EzCSCCRUDtoDBConverter {
    template <typename... Args>
    size_t CSVrow::setRowData(Args&&... args) {
        constexpr size_t numArgs = sizeof...(Args);
        size_t newRowIndex = 0;
        if (numArgs != columnCount) {
            throw std::invalid_argument("Row data size does not match column count: expected " +
                std::to_string(columnCount) + ", got " +
                std::to_string(numArgs));
        }

        // Use the current size of rowData as the new key
        newRowIndex = rowData.size();

        // Insert a new row into the unordered_map
        rowData[newRowIndex] = std::make_shared<std::vector<DynamicTypedValue>>(
            std::vector<DynamicTypedValue>{DynamicTypedValue(std::forward<Args>(args))...});

        return newRowIndex;
    }

    // Overloaded version for iterators
    template <typename InputIterator>
    size_t CSVrow::setRowData(InputIterator begin, InputIterator end) {
        size_t newRowIndex = 0;
        size_t numArgs = std::distance(begin, end);
        if (numArgs != columnCount) {
            throw std::invalid_argument("Row data size does not match column count: expected " +
                std::to_string(columnCount) + ", got " +
                std::to_string(numArgs));
        }

        // Use the current size of rowData as the new key
        newRowIndex = rowData.size();

        // Insert a new row using the range of values
        rowData[newRowIndex] = std::make_shared<std::vector<DynamicTypedValue>>(begin, end);
        return newRowIndex;
    }
}
#endif