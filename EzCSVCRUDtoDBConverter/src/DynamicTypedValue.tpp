// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef DYNAMICTYPEDVALUE_TPP
#define DYNAMICTYPEDVALUE_TPP

#include <stdexcept>
#include <utility>
#include <DynamicTypedValue.hpp>

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

    std::unique_ptr<ValueBase> clone() const override {
        return std::make_unique<ValueModel<T>>(*this);
    }

    T getValue() const {
        return storedValue;
    }
};

// Template constructor for any type
template <typename T>
DynamicTypedValue::DynamicTypedValue(T value)
    : storedValuePtr(std::make_unique<ValueModel<T>>(std::move(value))) {}

// Template method to access the underlying value
template <typename T>
T DynamicTypedValue::getValue() const {
    auto* model = dynamic_cast<ValueModel<T>*>(storedValuePtr.get());
    if (!model) {
        throw std::bad_cast();
    }
    return model->getValue();
}

#endif 