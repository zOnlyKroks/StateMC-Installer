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
    {"https://www.statemc.de/download/ts/StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin", "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin"},
    {"https://statemc.de/download/modpack/StateMC.zip", "C:\\StateInstallerTemp\\StateMC_Modpack.zip"}
};

const char* output_file_plugin = "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin";
const char* temp_directory = "C:\\StateInstallerTemp";

void excuteTeamspeakPluginInstall() {
    char* programFiles = nullptr;
    size_t len = 0;

    if (_dupenv_s(&programFiles, &len, "ProgramFiles") != 0 || programFiles == nullptr) {
        std::cerr << "Failed to get Program Files directory." << std::endl;
        return;
    }

    std::string teamspeakPath = std::string(programFiles) + "\\TeamSpeak 3 Client\\package_inst.exe";

    // Check if the file exists
    if (GetFileAttributesA(teamspeakPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "File not found: " << teamspeakPath << std::endl;
        return;
    }

    // Create the command line argument string
    std::string commandLine = "\"" + teamspeakPath + "\" \"" + output_file_plugin + "\"";

    // Execute the file
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(
        NULL,                          // No module name (use command line)
        const_cast<char*>(commandLine.c_str()), // Command line
        NULL,                          // Process handle not inheritable
        NULL,                          // Thread handle not inheritable
        FALSE,                         // Set handle inheritance to FALSE
        0,                             // No creation flags
        NULL,                          // Use parent's environment block
        NULL,                          // Use parent's starting directory 
        &si,                           // Pointer to STARTUPINFO structure
        &pi)                           // Pointer to PROCESS_INFORMATION structure
        ) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "package_inst.exe executed successfully." << std::endl;
}

void installJava() {
    const char* command = "msiexec /i \"C:\\StateInstallerTemp\\jre_installer.msi\" ADDLOCAL=FeatureMain,FeatureEnvironment,FeatureJarFileRunWith,FeatureJavaHome,FeatureOracleJavaSoft, INSTALLDIR=\"C:\\Program Files\\Java\\\"";
    system(command);
}

int main() {
    std::cout << "StateMC Installer" << std::endl;

    createDownloadDirectory("C:\\StateInstallerTemp\\");

    for (const auto& [url, fileName] : downloadUrls) {
        if (!downloadURL(url, fileName)) {
            std::cerr << "Failed to download: " << fileName << std::endl;
        }
    }

    //Java
    installJava();

    //Teamspeak
    system("C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe");

    //Plugin
    excuteTeamspeakPluginInstall();

    //Technic
    system("C:\\StateInstallerTemp\\TechnicLauncher.exe");

    //Clean temp dir
    deleteTempDirectory(temp_directory);

    std::cout << "If you see this, installation is finished and you can close this window." << std::endl;

    return 0;
}
