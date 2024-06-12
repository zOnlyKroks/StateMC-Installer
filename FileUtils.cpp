#include "FileUtils.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <math.h>
#include <thread>
#include <windows.h>
#include <filesystem>


fs::path findExecutable(const fs::path& rootDir, const std::string& exeName) {
    std::vector<fs::directory_entry> entries;

    std::copy(fs::recursive_directory_iterator(rootDir),
              fs::recursive_directory_iterator(),
              std::back_inserter(entries));

    auto it = std::find_if(entries.begin(), entries.end(), [&exeName](const fs::directory_entry& entry) {
        return entry.is_regular_file() && entry.path().filename() == exeName;
    });

    if (it != entries.end()) {
        return it->path();
    }

    return fs::path();  // Return an empty path if not found
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