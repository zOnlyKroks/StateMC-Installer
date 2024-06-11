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
#include "JavaHelper.h"
#include "DownloadUtils.h"
#include "TeamspeakHelper.h"
#include "TechnicHelper.h"

void handleSwitch() {
    system("cls");

    std::cout << "StateMC Installer" << std::endl;

    createDownloadDirectory("C:\\StateInstallerTemp\\");

    moveCursorToLine(18);
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

    handleSwitch();

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
