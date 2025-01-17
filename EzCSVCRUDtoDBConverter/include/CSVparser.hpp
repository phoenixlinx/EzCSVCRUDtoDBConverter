
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef    CSVPARSER_HPP_
#define    CSVPARSER_HPP_

#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <optional>
#include <filesystem>
#include <fstream>
#include "SystemMetadata.hpp"
namespace csv
{
    class Error : public std::runtime_error
    {

    public:
        Error(const std::string& msg) :
            std::runtime_error(std::string("CSVparser : ").append(msg))
        {
        }
    };

    class Row
    {
    public:
        Row(const std::vector<std::string>&);
        ~Row(void);

    public:
        size_t size(void) const;
        void push(const std::string&);
        bool set(const std::string&, const std::string&);

    private:
        const std::vector<std::string> _header;
        std::vector<std::string> _values;

    public:

        template<typename T>
        const T getValue(size_t pos) const
        {
            if (pos < _values.size())
            {
                T res;
                std::stringstream ss;
                ss << _values[pos];
                ss >> res;
                return res;
            }
            throw Error("can't return this value (doesn't exist)");
        }
        const std::string operator[](size_t) const;
        const std::string operator[](const std::string& valueName) const;
        friend std::ostream& operator<<(std::ostream& os, const Row& row);
        friend std::ofstream& operator<<(std::ofstream& os, const Row& row);
    };

    enum DataType {
        eFILE = 0,
        ePURE = 1
    };

    class Parser
    {

    public:
        Parser(const std::string&, const DataType& type = eFILE, char sep = ',');
        ~Parser(void);

    public:
        Row& getRow(size_t row) const;
        size_t rowCount(void) const;
        size_t columnCount(void) const;
        std::vector<std::string> getHeader(void) const;
        const std::string getHeaderElement(size_t pos) const;
        const std::string& getFileName(void) const;

    public:
        bool deleteRow(size_t row);
        bool addRow(size_t pos, const std::vector<std::string>&);
        void sync(void) const;

    protected:
        void parseHeader(void);
        void parseContent(void);

    private:
        std::string _file;
        const DataType _type;
        const char _sep;
        std::vector<std::string> _originalFile;
        std::vector<std::string> _header;
        std::vector<Row*> _content;

    public:
        Row& operator[](size_t row) const;
        static std::optional<std::filesystem::path> selectFile();
    };
   
    
}

#endif 
