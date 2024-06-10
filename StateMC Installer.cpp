#include <iostream>
#include <cmath>
#include <thread>
#include <windows.h>
#include <tchar.h>
#include <filesystem>
#include <optional>
#include <vector>
#include "Downloader.h"
#include "FileUtils.h"
#include "ProcessHelper.h"

namespace fs = std::filesystem;

const char* output_file_plugin = "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin";
const char* temp_directory = "C:\\StateInstallerTemp";

void executeTeamspeakPluginInstall() {
    std::cout << "Installing Teamspeak plugin..." << std::endl;
    char* programFiles = nullptr;
    size_t len = 0;

    if (_dupenv_s(&programFiles, &len, "ProgramFiles") != 0 || !programFiles) {
        std::cerr << "Failed to get Program Files directory." << std::endl;
        return;
    }

    std::unique_ptr<char, decltype(&free)> programFilesPtr(programFiles, free);

    std::string teamspeakPath = std::string(programFilesPtr.get()) + "\\TeamSpeak 3 Client\\package_inst.exe";
    if (GetFileAttributesA(teamspeakPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "File not found: " << teamspeakPath << std::endl;
        return;
    }

    std::string commandLine = "\"" + teamspeakPath + "\" \"" + output_file_plugin + "\"";

    if (!runProcess(commandLine)) {
        std::cerr << "Failed to install Teamspeak plugin." << std::endl;
    }
    else {
        std::cout << "Teamspeak plugin installed." << std::endl;
    }
}

void installJava() {
    std::cout << "Installing Java..." << std::endl;
    const char* command = "msiexec /i \"C:\\StateInstallerTemp\\jre_installer.msi\" ADDLOCAL=FeatureMain,FeatureEnvironment,FeatureJarFileRunWith,FeatureJavaHome,FeatureOracleJavaSoft INSTALLDIR=\"C:\\Program Files\\Java\\\"";
    if (!runProcess(command)) {
        std::cerr << "Failed to install Java." << std::endl;
    }
    else {
        std::cout << "Java installed." << std::endl;
    }
}

bool downloadFilesConcurrently(std::vector<std::pair<const char*, const char*>> downloadUrls) {
    std::vector<std::thread> downloadThreads;

    int console_line = 3;  // Starting line for download progress

    for (const auto& [url, fileName] : downloadUrls) {
        downloadThreads.emplace_back([url, fileName, console_line]() {
            if (!downloadURL(url, fileName, console_line)) {
                std::cerr << "Failed to download: " << fileName << std::endl;
            }
            });
        console_line += 4;  // Adjust line for next download progress
    }

    for (auto& thread : downloadThreads) {
        thread.join();
    }

    return true;
}

void executeTeamspeakInstaller() {
    std::cout << "Installing Teamspeak..." << std::endl;
    const char* command = "C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe";
    if (!runProcess(command)) {
        std::cerr << "Failed to install Teamspeak." << std::endl;
    }
    else {
        std::cout << "Teamspeak installed." << std::endl;
    }
}

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

void handleCreateDirectory() {
    createDownloadDirectory("C:\\StateInstallerTemp\\");
}

void finishUp() {
    std::cout << "Cleaning up..." << std::endl;
    deleteTempDirectory(temp_directory);
    std::cout << "Cleaned up." << std::endl;

    std::cout << "If you see this, installation is finished and you can close this window." << std::endl;
    std::cout << "Made with love by zOnlyKroks :3" << std::endl;
}

boolean doInstallJava = false;
boolean doExecuteTeamspeak = false;
boolean doExecuteTeamspeakPlugin = false;
boolean doExecuteTechnicLauncher = false;

int main() {
    std::cout << "StateMC Installer" << std::endl;

    std::vector<std::pair<const char*, const char*>> downloadUrls = {};

    char choice;

    std::cout << "Do you want to install Java? (y/n): ";
    std::cin >> choice;

    if (choice == 'y') {
        downloadUrls.push_back({ "https://api.adoptium.net/v3/installer/latest/8/ga/windows/x64/jre/hotspot/normal/eclipse?project=jdk", "C:\\StateInstallerTemp\\jre_installer.msi" });
        doInstallJava = true;
	}

    std::cout << "Do you want to install Teamspeak? (y/n): ";
    std::cin >> choice;

    if (choice == 'y') {
        downloadUrls.push_back({ "https://download.filepuma.com/files/chat-and-instant-messaging/teamspeak-client-64bit-/TeamSpeak_Client_(64bit)_v3.6.0.exe", "C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe" });
        doExecuteTeamspeak = true;
    }

    std::cout << "Do install Teamspeak Plugin? (y/n): ";
    std::cin >> choice;

    if (choice == 'y') {
        downloadUrls.push_back({ "https://www.statemc.de/download/ts/StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin", "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin" });
        doExecuteTeamspeakPlugin = true;
	}

    std::cout << "Do you want to execute Technic Launcher? (y/n): ";
    std::cin >> choice;

    if (choice == 'y') {
        downloadUrls.push_back({ "https://launcher.technicpack.net/launcher4/831/TechnicLauncher.exe", "C:\\StateInstallerTemp\\TechnicLauncher.exe" });
		doExecuteTechnicLauncher = true;
	}

    system("cls");

    std::cout << "StateMC Installer" << std::endl;

    handleCreateDirectory();

    moveCursorToLine(18);

    if (!downloadFilesConcurrently(downloadUrls)) {
        std::cerr << "Failed to download files." << std::endl;
        return -1;
    }

    // Java
    if (doInstallJava) {
        installJava();
    }

    // Teamspeak
    if (doExecuteTeamspeak) {
        executeTeamspeakInstaller();
    }

    // Plugin
    if (doExecuteTeamspeakPlugin) {
        executeTeamspeakPluginInstall();
    }

    // Move and execute Technic Launcher
    if (doExecuteTechnicLauncher) {
        moveAndExecuteTechnicLauncher();
    }

    // Clean temp dir
    finishUp();

    return 0;
}
