// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/DynamicTypedValue.hpp"
namespace EzCSCCRUDtoDBConverter {


    // Default constructor initializes the DynamicTypedValue with an empty string
// This ensures every object has a valid state, even when no type is explicitly specified.
    DynamicTypedValue::DynamicTypedValue()
        : storedValuePtr(std::make_shared<ValueModel<int>>(0)),
        typeIndex(typeid(int)) { // Default type index to std::string
    }

    // Copy constructor clones the underlying ValueBase
    // This ensures deep copying of the polymorphic object stored in storedValuePtr.
    DynamicTypedValue::DynamicTypedValue(const DynamicTypedValue& other)
        : storedValuePtr(other.storedValuePtr->clone()),
        typeIndex(typeid(*other.storedValuePtr)) {
    }
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
    std::type_index DynamicTypedValue::getTypeIndex() const {
        return (typeid(*storedValuePtr));
    }


    // Retrieves the name of the type currently stored in the DynamicTypedValue
    // This is useful for debugging or inspecting the type of the stored value at runtime.
    std::string DynamicTypedValue::getStoredTypeName() const {
        return storedValuePtr->getTypeName();
    }

    bool DynamicTypedValue::operator==(const DynamicTypedValue& other) const {
        return dispatchComparison(other, std::equal_to<>{});
    }

    bool DynamicTypedValue::operator<(const DynamicTypedValue& other) const {
        return dispatchComparison(other, std::less<>{});
    }

    bool DynamicTypedValue::operator>(const DynamicTypedValue& other) const {
        return dispatchComparison(other, std::greater<>{});
    }

    bool DynamicTypedValue::operator<=(const DynamicTypedValue& other) const {
        return dispatchComparison(other, std::less_equal<>{});
    }

    bool DynamicTypedValue::operator>=(const DynamicTypedValue& other) const {
        return dispatchComparison(other, std::greater_equal<>{});
    }
}