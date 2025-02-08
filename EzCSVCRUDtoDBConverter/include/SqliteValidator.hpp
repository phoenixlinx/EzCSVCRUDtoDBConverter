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

namespace EzCSCCRUDtoDBConverter {
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

        //--------------------------------------------------------------------------
 // 1) UTF-8 Byte Patterns and Masks
 //--------------------------------------------------------------------------
        static constexpr unsigned char UTF8_2_BYTE_MASK = 0xE0; // 1110 0000
        static constexpr unsigned char UTF8_2_BYTE_PATTERN = 0xC0; // 1100 0000

        static constexpr unsigned char UTF8_3_BYTE_MASK = 0xF0; // 1111 0000
        static constexpr unsigned char UTF8_3_BYTE_PATTERN = 0xE0; // 1110 0000

        static constexpr unsigned char UTF8_4_BYTE_MASK = 0xF8; // 1111 1000
        static constexpr unsigned char UTF8_4_BYTE_PATTERN = 0xF0; // 1111 0000

        //--------------------------------------------------------------------------
        // 2) Bit Masks for Extracting Lower Bits in Continuation Bytes
        //--------------------------------------------------------------------------
        static constexpr unsigned char LOWER_6_BITS_MASK = 0x3F; // 0011 1111
        static constexpr unsigned char LOWER_5_BITS_MASK = 0x1F; // 0001 1111
        static constexpr unsigned char LOWER_4_BITS_MASK = 0x0F; // 0000 1111
        static constexpr unsigned char LOWER_3_BITS_MASK = 0x07; // 0000 0111

        //--------------------------------------------------------------------------
        // 3) ASCII Boundary for Single-Byte UTF-8
        //--------------------------------------------------------------------------
        static constexpr unsigned char ASCII_7_BIT_MAX = 0x7F; // 0111 1111

        //--------------------------------------------------------------------------
        // 4) Minimum Required Byte Sequences
        //--------------------------------------------------------------------------
        static constexpr std::ptrdiff_t MIN_2_BYTE_SEQUENCE = 2;
        static constexpr std::ptrdiff_t MIN_3_BYTE_SEQUENCE = 3;
        static constexpr std::ptrdiff_t MIN_4_BYTE_SEQUENCE = 4;

        //--------------------------------------------------------------------------
        // 5) Shift Amounts When Building Codepoints
        //--------------------------------------------------------------------------
        static constexpr unsigned int SHIFT_6 = 6;
        static constexpr unsigned int SHIFT_12 = 12;
        static constexpr unsigned int SHIFT_18 = 18;

        //--------------------------------------------------------------------------
        // 6) "Printable" Codepoint Rules
        //    (Here you can define tab, space, and minimal printables.)
        //--------------------------------------------------------------------------
        static constexpr unsigned int ASCII_PRINTABLE_LOW = 0x20; // space is lowest
        // If you want, you could define an upper limit for "printable" or
        // handle extended Unicode ranges. This example is simplified.

        //--------------------------------------------------------------------------
        // 7) Maximum Allowed Key Length
        //--------------------------------------------------------------------------
        static constexpr std::size_t MAX_KEY_LENGTH = 255;
        // ------------------------------------------------
// Named constants for codepoint ranges
// ------------------------------------------------
        static constexpr unsigned int ASCII_TAB = 0x09;    // tab
        static constexpr unsigned int ASCII_SPACE = 0x20;    // space
        static constexpr unsigned int ASCII_EXCLAMATION_MARK = 0x21;    // '!'
        static constexpr unsigned int ASCII_TILDE = 0x7E;    // '~'

        static constexpr unsigned int UNICODE_NO_BREAK_SPACE = 0xA0;    // beginning of "Latin-1 Supplement" printables
        static constexpr unsigned int UNICODE_MAX_BEFORE_SURROGATES = 0xD7FF;  // end of first BMP block before surrogates
        static constexpr unsigned int UNICODE_SURROGATE_START = 0xE000;  // start of second BMP block after surrogates
        static constexpr unsigned int UNICODE_SURROGATE_END = 0xFFFD;  // typical last "valid" codepoint in BMP (ignoring U+FFFF, etc.)

        // For supplementary planes
        static constexpr unsigned int UNICODE_SUPPLEMENTARY_START = 0x10000;
        static constexpr unsigned int UNICODE_SUPPLEMENTARY_END = 0x10FFFF;

    };
}
#include "../src/SqliteValidator.tpp"
#endif


