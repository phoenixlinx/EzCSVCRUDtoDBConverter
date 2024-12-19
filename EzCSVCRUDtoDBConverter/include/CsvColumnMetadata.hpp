#ifndef CSV_COLUMN_METADATA_HPP
#define CSV_COLUMN_METADATA_HPP

#include <typeinfo>
#include <cstddef> // for size_t
class CsvColumnMetadata {
private:
    const std::type_info* type;  // Pointer to type info
    size_t index;               // Column index

    // Grant friendship to allow controlled access
    friend class CSVMetadata;

public:
    // Constructor
    CsvColumnMetadata(const std::type_info* columnType, size_t columnIndex);
    CsvColumnMetadata();
    // Public getters for controlled access
    const std::type_info* getType() const;
    size_t getIndex() const;
};

#endif 