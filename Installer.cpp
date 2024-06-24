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

boolean doInstallJava = false;
boolean doExecuteTeamspeak = false;
boolean doExecuteTeamspeakPlugin = false;
boolean doInstallModpack = false;
boolean moveTechnicLauncher = false;
boolean doLaunchTechnicLauncher = true;

void handleSwitch() {
    system("cls");

    std::cout << "StateMC Installer" << std::endl;

    createDownloadDirectory("C:\\StateInstallerTemp\\");
}

void start() {
    std::cout << "StateMC Installer" << std::endl;

    std::vector<std::pair<const char*, const char*>> downloadUrls = {};

    const std::string desktopPath = getDesktopPath().value();

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

    std::cout << "Do you want to install Technic Launcher? (y/n): ";
    std::cin >> choice;

    if (choice == 'y') {
        downloadUrls.push_back({ "https://launcher.technicpack.net/launcher4/831/TechnicLauncher.exe", "C:\\StateInstallerTemp\\TechnicLauncher.exe" });
        moveTechnicLauncher = true;
    }

    std::cout << "Do you want to install the modpack into Technic Launcher? BEWARE THIS DELETES ANY MODPACKS YOU ALREADY HAVE INSTALLED IN TECHNIC LAUNCHER! I HOLD NO LIABILITY IF YOU DELETE ANYTHING!!!! (y/n): ";
    std::cin >> choice;

    if(choice == 'y') {
		doInstallModpack = true;
	}

    std::cout << "Do you want to launch TechnicLauncher after we are finished here? (y/n): ";
    std::cin >> choice;

    if (choice == 'y') {
        doLaunchTechnicLauncher = true;
    }

    handleSwitch();

    moveCursorToLine(18);

    if (!downloadFilesConcurrently(downloadUrls)) {
        std::cerr << "Failed to download files." << std::endl;
        return;
    }

    handleSwitch();

    moveCursorToLine(2);

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

    if (doInstallModpack) {
        createNeccesaryDirectories();
    }

    if (moveTechnicLauncher) {
        copyTechnicLauncher(desktopPath);
    }

    if (doLaunchTechnicLauncher) {
        executeTechnicLauncher(desktopPath);
    }

    // Clean temp dir
    finishUp();
}

