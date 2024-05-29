#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <math.h>
#include <cstdlib> // for system function
#include <chrono>
#include <thread>
#include <windows.h>
#include <tchar.h>
#include <filesystem>
#include <optional>
#include "Downloader.h"
#include "FileUtils.h"

namespace fs = std::filesystem;

const std::vector<std::pair<const char*, const char*>> downloadUrls = {
    {"https://api.adoptium.net/v3/installer/latest/8/ga/windows/x64/jre/hotspot/normal/eclipse?project=jdk", "C:\\StateInstallerTemp\\jre_installer.msi"},
    {"https://launcher.technicpack.net/launcher4/831/TechnicLauncher.exe", "C:\\StateInstallerTemp\\TechnicLauncher.exe"},
    {"https://download.filepuma.com/files/chat-and-instant-messaging/teamspeak-client-64bit-/TeamSpeak_Client_(64bit)_v3.6.0.exe", "C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe"},
    {"https://www.statemc.de/download/ts/StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin", "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin"}
};

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
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessA(nullptr, commandLine.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    std::cout << "Teamspeak plugin installed." << std::endl;
}

void installJava() {
    std::cout << "Installing Java..." << std::endl;
    const char* command = "msiexec /i \"C:\\StateInstallerTemp\\jre_installer.msi\" ADDLOCAL=FeatureMain,FeatureEnvironment,FeatureJarFileRunWith,FeatureJavaHome,FeatureOracleJavaSoft, INSTALLDIR=\"C:\\Program Files\\Java\\\"";
    system(command);
    std::cout << "Java installed." << std::endl;
}

bool downloadFilesConcurrently() {
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

int main() {
    std::cout << "StateMC Installer" << std::endl;

    createDownloadDirectory("C:\\StateInstallerTemp\\");

    if (!downloadFilesConcurrently()) {
        std::cerr << "Failed to download files." << std::endl;
        return 1;
    }

    moveCursorToLine(18);

    //Java
    installJava();

    //Teamspeak
    std::cout << "Installing Teamspeak..." << std::endl;
    system("C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe");
    std::cout << "Teamspeak installed." << std::endl;

    //Plugin
    executeTeamspeakPluginInstall();

    //Technic
    std::cout << "Installing Technic Launcher..." << std::endl;
    system("C:\\StateInstallerTemp\\TechnicLauncher.exe");
    std::cout << "Technic Launcher installed." << std::endl;

    //Clean temp dir
    std::cout << "Cleaning up..." << std::endl;
    deleteTempDirectory(temp_directory);
    std::cout << "Cleaned up." << std::endl;

    std::cout << "If you see this, installation is finished and you can close this window." << std::endl;
    std::cout << "Made with love by zOnlyKroks :3" << std::endl;

    return 0;
}
