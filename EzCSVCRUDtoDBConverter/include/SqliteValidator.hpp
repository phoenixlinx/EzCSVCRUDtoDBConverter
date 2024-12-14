// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef SQLITE_VALIDATOR_HPP
#define SQLITE_VALIDATOR_HPP

#include <string>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <type_traits> 
#include <ranges>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <map>
// This class was developed with assistance from ChatGPT 4o. See README.md.
// Reference: OpenAI (2024)
class SqliteValidator {
public:

   

    template <std::ranges::input_range Range>
    static inline bool isPrintableUtf8(const Range& keys)
    requires std::is_same_v<std::ranges::range_value_t<Range>, std::string>;
  
    
   

    static inline bool isPrintableUtf8(const std::string& key);

    static inline std::unordered_map<std::string, std::string> quoteColumnName(const std::vector<std::string>& keys);

    static inline std::string quoteColumnName(const std::string& key);

private:
    // Prevent instantiation (utility class)
    SqliteValidator() = default;

 
    static inline bool validateLength(const std::string& name);
    static inline bool isPrintableCodepoint(unsigned int codepoint);

    static const size_t MAX_NAME_LENGTH = 255;


 ;

};
#include "../src/SqliteValidator.tpp"
#endif


