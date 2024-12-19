#include <CsvColumnMetadata.hpp>


CsvColumnMetadata::CsvColumnMetadata(const std::type_info* columnType, size_t columnIndex)
    : type(columnType), index(columnIndex) {}


CsvColumnMetadata::CsvColumnMetadata()
    : index(0) {
}
const std::type_info* CsvColumnMetadata::getType() const {
    return type;
}

size_t CsvColumnMetadata::getIndex() const {
    return index;
}
