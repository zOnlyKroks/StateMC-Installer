#include "TechnicHelper.h"

#include <iostream>
#include <filesystem>
#include "ProcessHelper.h"

namespace fs = std::filesystem;

void moveAndExecuteTechnicLauncher() {
    // Get desktop path, copy from temp to desktop
    char* desktopPath = nullptr;
    size_t len = 0;

    if (_dupenv_s(&desktopPath, &len, "USERPROFILE") != 0 || !desktopPath) {
        std::cerr << "Failed to get desktop directory." << std::endl;
        return;
    }

    std::unique_ptr<char, decltype(&free)> desktopPathPtr(desktopPath, free);

    std::string desktopPathString = std::string(desktopPathPtr.get()) + "\\Desktop\\TechnicLauncher.exe";
    try {
        fs::copy("C:\\StateInstallerTemp\\TechnicLauncher.exe", desktopPathString, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to copy TechnicLauncher.exe to desktop: " << e.what() << std::endl;
        return;
    }

    // Execute Technic Launcher
    std::cout << "Executing Technic Launcher..." << std::endl;
    if (!runProcess(desktopPathString)) {
        std::cerr << "Failed to execute Technic Launcher." << std::endl;
    }
    else {
        std::cout << "Technic Launcher executed." << std::endl;
    }
}