#include "SystemMetadata.hpp"
#include <stdexcept>

#include "SystemMetadata.hpp"

SystemMetadata::Platform SystemMetadata::getPlatform() {
#ifdef _WIN32
    return Platform::Windows;
#elif __linux__
    return Platform::Linux;
#elif __APPLE__
    return Platform::macOS;
#else
    return Platform::Unknown;
#endif
}

bool SystemMetadata::isGuiAvailable() {
#ifdef _WIN32
    return true; 
#elif __linux__
    return std::getenv("DISPLAY") != nullptr;
#elif __APPLE__
    return true;
#else
    return false;
#endif
}


std::optional<std::filesystem::path> SystemMetadata::selectFile() {
    if (!SystemMetadata::isGuiAvailable()) {
        throw std::runtime_error("GUI environment is not available.");
    }

    const char* filterDescription = "CSV Files";
    const char* filterPattern = "*.csv";

#ifdef _WIN32
    wchar_t fileName[MAX_PATH] = { 0 };
    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);

    std::wstring filter = std::wstring(filterDescription, filterDescription + strlen(filterDescription)) + L'\0' +
        std::wstring(filterPattern, filterPattern + strlen(filterPattern)) + L'\0';

    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        return std::filesystem::path(fileName);
    }

#elif __linux__
   
    //TODO: Fix file names with spaces bug. 
    const char* command = "zenity --file-selection --file-filter=\"CSV Files | *.csv\"";
    const size_t maxSize = 256;
    char buffer[maxSixe + 1]; 
    std::string result;
    //Open file explorere Window and limit selection to files with a .csv extension.
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to execute Zenity command.");
    }
 
    while (std::fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer; // Append each chunk to the result
    }

    // Check if  truncation occurred.
   
    if (result.size() > maxSize) {
        throw std::runtime_error("Input exceeds the allowed maximum size.");
    }
    
    
    if (result.empty()) {
        return std::nullopt;
    }
    else {
        return std::filesystem::path(result);
    }

#else
    throw std::runtime_error("Unsupported platform.");
#endif

    return std::nullopt; 
}
