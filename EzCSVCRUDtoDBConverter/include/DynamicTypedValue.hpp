// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef DYNAMICTYPEDVALUE_HPP
#define DYNAMICTYPEDVALUE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <variant>
class DynamicTypedValue {
private:

    using ValueType = std::variant<int, double, std::string>;
    // Abstract base class for type erasure
    struct ValueBase {
        virtual ~ValueBase() = default;
        virtual std::string getTypeName() const = 0;
        virtual void printValue(std::ostream& os) const = 0;
        virtual std::shared_ptr<ValueBase> clone() const = 0;
    };

    // Concrete implementation for specific types
    template <typename T>
    struct ValueModel;

    std::shared_ptr<ValueBase> storedValuePtr; // Pointer to the polymorphic base

public:
    // Default constructor
    DynamicTypedValue();
    
  



    template <typename T>
    explicit DynamicTypedValue(T value);

    DynamicTypedValue(const DynamicTypedValue& other);

    std::string getTypeAsString() const;

    DynamicTypedValue& operator=(const DynamicTypedValue& other);

    // Overloaded stream operator for printing
    friend std::ostream& operator<<(std::ostream& os, const DynamicTypedValue& value);
   
 

    template <typename T>
    inline operator T() const;



    template <typename T>
    T getValue() const;


    std::string getStoredTypeName() const;
    bool operator==(const DynamicTypedValue& other) const;
    bool operator<(const DynamicTypedValue& other) const;
    bool operator>(const DynamicTypedValue& other) const;
    ValueType data;

};

#include "../src/DynamicTypedValue.tpp"

#endif 
