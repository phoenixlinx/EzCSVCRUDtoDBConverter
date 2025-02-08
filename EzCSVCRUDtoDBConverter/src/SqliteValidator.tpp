// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#ifndef SQLITE_VALIDATOR_TPP
#define SQLITE_VALIDATOR_TPP
#include <SqliteValidator.hpp>

namespace EzCSCCRUDtoDBConverter {
    // Validates the length of a given name (table or column) and throws an exception if it exceeds the maximum allowed length.
    inline bool SqliteValidator::validateLength(const std::string& name) {
        if ((!name.empty()) && (name.length() <= MAX_NAME_LENGTH)) {
            return true;
        }
        else {
            throw std::invalid_argument(" Table or column name exceeds the maximum length of " + std::to_string(MAX_NAME_LENGTH) + " characters.");
        }

    }


    inline bool SqliteValidator::isPrintableCodepoint(unsigned int codepoint)
    {
        // This logic is exactly what you posted, but all numeric literals
        // are replaced by named constants:
        return
            (codepoint == ASCII_TAB) ||
            (codepoint == ASCII_SPACE) ||
            (
                (codepoint >= ASCII_EXCLAMATION_MARK) &&
                (codepoint <= ASCII_TILDE)
                ) ||
            (
                (codepoint >= UNICODE_NO_BREAK_SPACE) &&
                (codepoint <= UNICODE_MAX_BEFORE_SURROGATES)
                ) ||
            (
                (codepoint >= UNICODE_SURROGATE_START) &&
                (codepoint <= UNICODE_SURROGATE_END)
                ) ||
            (
                (codepoint >= UNICODE_SUPPLEMENTARY_START) &&
                (codepoint <= UNICODE_SUPPLEMENTARY_END)
                );
    }



    template <std::ranges::input_range Range>
   inline   bool SqliteValidator::isPrintableUtf8(const Range& keys)
        requires std::is_same_v<std::ranges::range_value_t<Range>, std::string>
    {
        // Defensive check, typically covered by the requires-clause:
        if (!std::is_same_v<std::ranges::range_value_t<Range>, std::string>) {
            std::cerr << "Error: Invalid type passed to isPrintableUtf8.\n";
            return false;
        }

        // Process each std::string in the Range
        for (const auto& key : keys) {

            // 1) Check length constraints
            if (!validateLength(key)) {
                return false;
            }

            // 2) Convert the entire std::string -> const unsigned char* once
            const unsigned char* dataPtr =
                reinterpret_cast<const unsigned char*>(key.data());

            const std::size_t totalSize = key.size();
            std::size_t currentIndex = 0;

            // 3) Iterate through all bytes in the string
            while (currentIndex < totalSize) {

                unsigned char firstByte = dataPtr[currentIndex];
                unsigned int codepoint = 0;

                // -- Single-byte (ASCII) range --
                if (firstByte <= ASCII_7_BIT_MAX) {
                    codepoint = firstByte;
                    currentIndex += 1;
                }
                // -- Two-byte UTF-8 sequence: 110xxxxx 10xxxxxx --
                else if (((firstByte & UTF8_2_BYTE_MASK) == UTF8_2_BYTE_PATTERN) &&
                    ((totalSize - currentIndex) >= MIN_2_BYTE_SEQUENCE))
                {
                    unsigned char secondByte = dataPtr[currentIndex + 1];
                    codepoint =
                        ((firstByte & LOWER_5_BITS_MASK) << SHIFT_6) |
                        (secondByte & LOWER_6_BITS_MASK);

                    currentIndex += 2;
                }
                // -- Three-byte UTF-8 sequence: 1110xxxx 10xxxxxx 10xxxxxx --
                else if (((firstByte & UTF8_3_BYTE_MASK) == UTF8_3_BYTE_PATTERN) &&
                    ((totalSize - currentIndex) >= MIN_3_BYTE_SEQUENCE))
                {
                    unsigned char secondByte = dataPtr[currentIndex + 1];
                    unsigned char thirdByte = dataPtr[currentIndex + 2];

                    codepoint =
                        ((firstByte & LOWER_4_BITS_MASK) << SHIFT_12) |
                        ((secondByte & LOWER_6_BITS_MASK) << SHIFT_6) |
                        (thirdByte & LOWER_6_BITS_MASK);

                    currentIndex += 3;
                }
                // -- Four-byte UTF-8 sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx --
                else if (((firstByte & UTF8_4_BYTE_MASK) == UTF8_4_BYTE_PATTERN) &&
                    ((totalSize - currentIndex) >= MIN_4_BYTE_SEQUENCE))
                {
                    unsigned char secondByte = dataPtr[currentIndex + 1];
                    unsigned char thirdByte = dataPtr[currentIndex + 2];
                    unsigned char fourthByte = dataPtr[currentIndex + 3];

                    codepoint =
                        ((firstByte & LOWER_3_BITS_MASK) << SHIFT_18) |
                        ((secondByte & LOWER_6_BITS_MASK) << SHIFT_12) |
                        ((thirdByte & LOWER_6_BITS_MASK) << SHIFT_6) |
                        (fourthByte & LOWER_6_BITS_MASK);

                    currentIndex += 4;
                }
                else {
                    // Invalid sequence or insufficient bytes remain
                    return false;
                }

                // 4) Finally, check if the codepoint is printable
                if (!isPrintableCodepoint(codepoint)) {
                    return false;
                }
            } 
        } 

        return true; // All keys validated successfully
    }

  

    inline bool SqliteValidator::isPrintableUtf8(const std::string& key) {
        // Wrap the single string in a range and call the range-based implementation
        return isPrintableUtf8(std::vector<std::string>{key});
    }




    // Quotes column and table name for use in SQL statements
    // It also escapes any internal double quotes.


    inline std::unordered_map<std::string, std::string> SqliteValidator::quoteColumnName(const std::vector<std::string>& keys)
    {

        std::unordered_map<std::string, std::string> quotedKeyMap;
        for (const auto& key : keys) {
            quotedKeyMap[key] = quoteColumnName(key);
        }
        return quotedKeyMap;



    }

    inline std::string SqliteValidator::quoteColumnName(const std::string& key)
    {
        std::string quoted = "\"";
        for (char c : key) {
            if (c == '"') {
                quoted += "\"\""; // Escape double quotes
            }
            else {
                quoted += c;
            }
        }
        quoted += "\"";
        return quoted;
    }


}


#endif


