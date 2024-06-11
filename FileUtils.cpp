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
    if (!CreateDirectoryW(widestr.c_str(), nullptr)) {
        DWORD error = GetLastError();
        if (error != ERROR_ALREADY_EXISTS) {
            std::cerr << "Error creating directory: " << error << std::endl;
        }
    }
}

void finishUp() {
    std::cout << "Cleaning up..." << std::endl;
    deleteTempDirectory("C:\\StateInstallerTemp");
    std::cout << "Cleaned up." << std::endl;

    std::cout << "If you see this, installation is finished and you can close this window." << std::endl;
    std::cout << "Made with love by zOnlyKroks :3" << std::endl;
}