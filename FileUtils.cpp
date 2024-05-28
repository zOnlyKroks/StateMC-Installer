#include "FileUtils.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <math.h>
#include <thread>
#include <windows.h>
#include <filesystem>

std::optional<fs::path> findExeFile(const fs::path& rootDir, const std::string& exeName) {
    try {
        if (!fs::exists(rootDir) || !fs::is_directory(rootDir)) {
            std::cerr << "Invalid root directory: " << rootDir << std::endl;
            return std::nullopt;
        }

        for (const auto& entry : fs::recursive_directory_iterator(rootDir)) {
            if (entry.is_regular_file() && entry.path().filename() == exeName) {
                return entry.path();
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return std::nullopt;
    }

    return std::nullopt;
}

void deleteTempDirectory(const std::string& path) {
    try {
        fs::remove_all(path);
        std::cout << "Temporary directory deleted: " << path << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error deleting temporary directory: " << e.what() << std::endl;
    }
}

void createDownloadDirectory(const std::string& path) {
    std::wstring widestr = std::wstring(path.begin(), path.end());
    CreateDirectory(widestr.c_str(), nullptr);
}