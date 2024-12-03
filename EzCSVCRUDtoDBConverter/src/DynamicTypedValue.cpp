// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/DynamicTypedValue.hpp"

// Default constructor initializes the DynamicTypedValue with an empty string
// This ensures every object has a valid state, even when no type is explicitly specified.
DynamicTypedValue::DynamicTypedValue()
    : storedValuePtr(std::make_shared<ValueModel<std::string>>("")) {}

// Copy constructor clones the underlying ValueBase
// This ensures deep copying of the polymorphic object stored in storedValuePtr.
DynamicTypedValue::DynamicTypedValue(const DynamicTypedValue& other)
    : storedValuePtr(other.storedValuePtr->clone()) {}

// Copy assignment operator replaces the current ValueBase with a clone of the other's ValueBase
// It avoids self-assignment and ensures the current object is updated correctly.
DynamicTypedValue& DynamicTypedValue::operator=(const DynamicTypedValue& other) {
    if (this != &other) { // Check for self-assignment
        storedValuePtr = other.storedValuePtr->clone(); // Deep copy the polymorphic object
    }
    return *this;
}

// Stream operator prints the stored value to the provided output stream
// Uses the polymorphic printValue method to ensure the correct value type is printed.
std::ostream& operator<<(std::ostream& os, const DynamicTypedValue& value) {
    value.storedValuePtr->printValue(os);
    return os;
}

// Retrieves the name of the type currently stored in the DynamicTypedValue
// This is useful for debugging or inspecting the type of the stored value at runtime.
std::string DynamicTypedValue::getStoredTypeName() const {
    return storedValuePtr->getTypeName();
}
