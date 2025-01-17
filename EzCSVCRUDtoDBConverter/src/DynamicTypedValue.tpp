// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef DYNAMICTYPEDVALUE_TPP
#define DYNAMICTYPEDVALUE_TPP

#include <stdexcept>
#include <utility>
#include <DynamicTypedValue.hpp>
namespace EzCSCCRUDtoDBConverter {
    // Template struct for specific value models
    template <typename T>
    struct DynamicTypedValue::ValueModel : ValueBase {
        T storedValue;

        explicit ValueModel(T value) : storedValue(std::move(value)) {}

        std::string getTypeName() const override {
            return typeid(T).name();
        }



        void printValue(std::ostream& os) const override {
            os << storedValue;
        }

        std::shared_ptr<ValueBase> clone() const override {
            return std::make_shared<ValueModel<T>>(*this);
        }

        T getValue() const {
            return storedValue;
        }
    };

    // Template constructor for any type
    template <typename T>
    DynamicTypedValue::DynamicTypedValue(T value)
        : storedValuePtr(std::make_shared<ValueModel<T>>(std::move(value))),
        typeIndex(typeid(T)) { // Capture the type of the original object
    }
   


    template<typename T>
    inline DynamicTypedValue::operator T() const {
        if (!std::holds_alternative<T>(data)) {
            throw std::runtime_error("Type mismatch");
        }
        return std::get<T>(data);
    }

  
    // Template method to access the underlying value
    template <typename T>
    T DynamicTypedValue::getValue() const {
        auto* model = dynamic_cast<ValueModel<T>*>(storedValuePtr.get());
        if (!model) {
            throw std::bad_cast();
        }
        return model->getValue();
    }

 

    template <typename Op>
    bool DynamicTypedValue::dispatchComparison(const DynamicTypedValue& other, Op operation) const {
        if (getStoredTypeName() != other.getStoredTypeName()) {
            throw std::runtime_error("Cannot  compare DynamicTypedValues with different types.");
        }

        // Lambda for safely casting and comparing
        auto compareValues = [&](auto* left, auto* right) -> bool {
            if (!left || !right) {
                throw std::runtime_error("Nullptr detected during comparison in DynamicTypedValue.");
            }
            return operation(left->getValue(), right->getValue());
            };

        // Dispatch table for supported types
        if (auto* leftString = dynamic_cast<ValueModel<std::string>*>(storedValuePtr.get())) {
            return compareValues(leftString, dynamic_cast<ValueModel<std::string>*>(other.storedValuePtr.get()));
        }
        else if (auto* leftInt = dynamic_cast<ValueModel<int>*>(storedValuePtr.get())) {
            return compareValues(leftInt, dynamic_cast<ValueModel<int>*>(other.storedValuePtr.get()));
        }
        else if (auto* leftDouble = dynamic_cast<ValueModel<double>*>(storedValuePtr.get())) {
            return compareValues(leftDouble, dynamic_cast<ValueModel<double>*>(other.storedValuePtr.get()));
        }
        else {
            throw std::runtime_error("Unsupported type for comparison in DynamicTypedValue.");
        }


    }
}
#endif 