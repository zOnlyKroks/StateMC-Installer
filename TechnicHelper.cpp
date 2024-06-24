#define _CRT_SECURE_NO_WARNINGS

#include "TechnicHelper.h"

#include <iostream>
#include <filesystem>
#include <optional>
#include <cstdlib>
#include <fstream>  
#include "ProcessHelper.h"
#include "Downloader.h"
#include "FileUtils.h"
#include <zip.h>

namespace fs = std::filesystem;

bool copyTechnicLauncher(const std::string& destination) {
    try {
        fs::copy("C:\\StateInstallerTemp\\TechnicLauncher.exe", destination, fs::copy_options::overwrite_existing);
        return true;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to copy TechnicLauncher.exe to desktop: " << e.what() << std::endl;
        return false;
    }
}

void executeTechnicLauncher(const std::string& path) {
    std::cout << "Executing Technic Launcher..." << std::endl;
    if (!runProcess(path)) {
        std::cerr << "Failed to execute Technic Launcher." << std::endl;
    }
    else {
        std::cout << "Technic Launcher executed." << std::endl;
    }
}

std::optional<std::string> getDesktopPath() {
    char* desktopPath = nullptr;
    size_t len = 0;

    if (_dupenv_s(&desktopPath, &len, "USERPROFILE") != 0 || !desktopPath) {
        return std::nullopt;
    }

    std::unique_ptr<char, decltype(&free)> desktopPathPtr(desktopPath, free);
    return std::string(desktopPathPtr.get()) + "\\Desktop\\TechnicLauncher.exe";
}

bool createNeccesaryDirectories() {
    const std::string dotTechnicPath = std::string(std::getenv("APPDATA")) + "\\.technic\\";

    const std::string modpacksPath = dotTechnicPath + "modpacks\\";

    createFolderIfNoExist(dotTechnicPath);
    createFolderIfNoExist(modpacksPath);

    const std::string installedModpacksFileWithoutEnding = dotTechnicPath + "installedPacks";
    std::ofstream outFile = std::ofstream(installedModpacksFileWithoutEnding);
    handleCreateInstalledModpackFile(dotTechnicPath, outFile);


    return true;
}

void handleCreateInstalledModpackFile(const std::string& dotTechnicPath, std::ofstream& outFile) {
    std::ifstream inFile(dotTechnicPath);

    outFile <<
        R"({
                "installedPacks": {
                    "statemc-roleplay-server": {
                    "name": "statemc-roleplay-server",
                    "build": "recommended",
                    "directory": "%MODPACKS%\\statemc-roleplay-server"
                }
                },
                "byIndex": [
                    "statemc-roleplay-server"
                ],
                "selected": "statemc-roleplay-server"
            })";

    outFile.close();
}

