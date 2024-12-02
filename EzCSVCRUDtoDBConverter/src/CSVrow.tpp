// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef CSVROW_TPP
#define CSVROW_TPP
#include<CSVrow.hpp>
// Template method to set row data using variadic arguments
template <typename... Args>
void CSVrow::setRowData(Args&&... args) {
    constexpr size_t numArgs = sizeof...(Args);
    if (numArgs != columnCount) {
        throw std::invalid_argument("Row data size does not match column count: expected " +
            std::to_string(columnCount) + ", got " +
            std::to_string(numArgs));
    }
    
    // Use the current size of rowData as the new key
    size_t newRowKey = rowData.size();

    // Insert a new row into the unordered_map
    rowData[newRowKey] = std::make_shared<std::vector<DynamicTypedValue>>(
        std::vector<DynamicTypedValue>{DynamicTypedValue(std::forward<Args>(args))...}
    );
}
#endif