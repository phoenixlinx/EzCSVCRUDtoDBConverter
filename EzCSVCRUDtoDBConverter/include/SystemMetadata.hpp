#ifndef SYSTEM_METADATA_HPP
#define SYSTEM_METADATA_HPP

#include <string>
#include <optional>
#include <filesystem>
#include <stdexcept>
#include <cstdio> // For popen, pclose, FILE on Linux/macOS
#include <array>  // For buffer usage

// Platform-specific includes
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>   // Core Windows API
#include <commdlg.h>   // For file dialogs (OPENFILENAMEW, GetOpenFileNameW)
#endif

class SystemMetadata {
public:
    // Represents the platform type
    enum class Platform {
        Windows,
        Linux,
        macOS,
        Unknown
    };

    // Returns the platform (Windows, Linux, macOS, or Unknown)
    static Platform getPlatform();

    // Checks if a GUI environment is available
    static bool isGuiAvailable();

    // Optional helper: File dialog for selecting a file (Windows-specific)
    static std::optional<std::filesystem::path> selectFile();

private:
    SystemMetadata() = delete; // Prevent instantiation
};

#endif // SYSTEM_METADATA_HPP
