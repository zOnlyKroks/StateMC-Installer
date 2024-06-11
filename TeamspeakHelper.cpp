#include "TeamspeakHelper.h"

#include <iostream>
#include <windows.h>
#include "ProcessHelper.h"

const char* output_file_plugin = "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin";

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