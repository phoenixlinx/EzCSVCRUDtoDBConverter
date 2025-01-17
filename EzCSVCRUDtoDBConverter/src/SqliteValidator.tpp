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


    inline bool SqliteValidator::isPrintableCodepoint(unsigned int codepoint) {
        return codepoint == 0x09 || codepoint == 0x20 ||
            (codepoint >= 0x21 && codepoint <= 0x7E) || // Printable ASCII range
            (codepoint >= 0xA0 && codepoint <= 0xD7FF) || // General printable range
            (codepoint >= 0xE000 && codepoint <= 0xFFFD) || // Exclude surrogate pairs
            (codepoint >= 0x10000 && codepoint <= 0x10FFFF); // Supplementary planes

    }


    template <std::ranges::input_range Range>
    inline bool SqliteValidator::isPrintableUtf8(const Range& keys)
        requires std::is_same_v<std::ranges::range_value_t<Range>, std::string>
    {
        // Additional runtime check for defense in depth
        if (!std::is_same_v<std::ranges::range_value_t<Range>, std::string>) {
            std::cerr << "Error: Invalid type passed to isPrintableUtf8." << std::endl;
            return false; // Fail gracefully
        }

        unsigned char c;
        unsigned int codepoint;
        // Iterate through each key in the provided range
        for (const auto& key : keys) {
            if (!validateLength(key)) {
                return false;

            }
            auto keyIterator = key.begin(); // Iterator to traverse through the current key
            auto remainingDistance = std::distance(keyIterator, key.end()); // Cache the distance to avoid repeated calculations

            // Process each character in the key
            while (keyIterator != key.end()) {
                c = *keyIterator; // Treat as unsigned byte


                // Decode UTF-8 character and determine its codepoint
                if (c <= 0x7F) { // Single-byte character (ASCII range: 0x00 to 0x7F)
                    codepoint = c; // Directly assign ASCII value to codepoint
                    ++keyIterator; // Move to the next byte
                    --remainingDistance; // Update remaining distance
                }
                else if ((c & 0xE0) == 0xC0 && remainingDistance >= 2) {
                    // Two-byte UTF-8 sequence: 110xxxxx 10xxxxxx
                    codepoint = ((c & 0x1F) << 6) | (*(keyIterator + 1) & 0x3F);
                    keyIterator += 2; // Advance iterator by 2 bytes
                    remainingDistance -= 2; // Update remaining distance
                }
                else if ((c & 0xF0) == 0xE0 && remainingDistance >= 3) {
                    // Three-byte UTF-8 sequence: 1110xxxx 10xxxxxx 10xxxxxx
                    codepoint = ((c & 0x0F) << 12) |
                        ((*(keyIterator + 1) & 0x3F) << 6) |
                        (*(keyIterator + 2) & 0x3F);
                    keyIterator += 3; // Advance iterator by 3 bytes
                    remainingDistance -= 3; // Update remaining distance
                }
                else if ((c & 0xF8) == 0xF0 && remainingDistance >= 4) {
                    // Four-byte UTF-8 sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    codepoint = ((c & 0x07) << 18) |
                        ((*(keyIterator + 1) & 0x3F) << 12) |
                        ((*(keyIterator + 2) & 0x3F) << 6) |
                        (*(keyIterator + 3) & 0x3F);
                    keyIterator += 4; // Advance iterator by 4 bytes
                    remainingDistance -= 4; // Update remaining distance
                }
                else {
                    return false; // Reject invalid UTF-8 sequences
                }

                // Validate if the character is printable or allowed (tab and space included)
                if (!isPrintableCodepoint(codepoint)) {
                    return false; // Reject non-printable or disallowed characters
                }
            }
        }
        return true; // All keys are valid UTF-8 and meet the printable criteria
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


