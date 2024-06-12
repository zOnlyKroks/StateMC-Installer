#include "TechnicHelper.h"

#include <iostream>
#include <filesystem>
#include <optional>
#include <cstdlib>
#include "ProcessHelper.h"

namespace fs = std::filesystem;

std::optional<std::string> getDesktopPath() {
    char* desktopPath = nullptr;
    size_t len = 0;

    if (_dupenv_s(&desktopPath, &len, "USERPROFILE") != 0 || !desktopPath) {
        return std::nullopt;
    }

    std::unique_ptr<char, decltype(&free)> desktopPathPtr(desktopPath, free);
    return std::string(desktopPathPtr.get()) + "\\Desktop\\TechnicLauncher.exe";
}

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

void moveAndExecuteTechnicLauncher() {
    auto desktopPathOpt = getDesktopPath();
    if (!desktopPathOpt) {
        std::cerr << "Failed to get desktop directory." << std::endl;
        return;
    }

    const std::string& desktopPath = desktopPathOpt.value();
    if (copyTechnicLauncher(desktopPath)) {
        executeTechnicLauncher(desktopPath);
    }
}
