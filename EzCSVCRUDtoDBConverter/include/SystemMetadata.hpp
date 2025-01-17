// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef SYSTEM_METADATA_HPP
#define SYSTEM_METADATA_HPP

#include <string>
#include <optional>
#include <filesystem>
#include <stdexcept>
#include <cstdio> 
#include <array>  

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>   
#include <commdlg.h> 
#endif
namespace EzCSCCRUDtoDBConverter {
    class SystemMetadata {
    public:
        static constexpr const char* filterDescription = "CSV Files";
        static constexpr const char* filterPattern = "*.csv";
        //Represents the platform type
        enum class Platform {
            Windows,
            Linux,
            macOS,
            Unknown
        };

        //Returns the platform (Windows, Linux, macOS, or Unknown)
        static Platform getPlatform();

        //Checks if a GUI environment is available.
        static bool isGuiAvailable();

        static std::optional<std::filesystem::path> selectFile();

    private:
        SystemMetadata() = delete;
    };
}
#endif
