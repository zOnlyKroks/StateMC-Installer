// File path: main.cpp

#define CURL_STATICLIB

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

int nb_bar;
double last_progress, progress_bar_adv;

namespace fs = std::filesystem;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

const std::vector<std::pair<const char*, const char*>> downloadUrls = {
    {"https://api.adoptium.net/v3/installer/latest/8/ga/windows/x64/jre/hotspot/normal/eclipse?project=jdk", "C:\\StateInstallerTemp\\jre_installer.msi"},
    {"https://launcher.technicpack.net/launcher4/831/TechnicLauncher.exe", "C:\\StateInstallerTemp\\TechnicLauncher.exe"},
    {"https://download.filepuma.com/files/chat-and-instant-messaging/teamspeak-client-64bit-/TeamSpeak_Client_(64bit)_v3.6.0.exe", "C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe"},
    {"https://www.statemc.de/download/ts/StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin", "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin"}
};

const char* output_file_plugin = "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin";
const char* temp_directory = "C:\\StateInstallerTemp";

int progress_bar(void* bar, double t, double d) {
    if (last_progress != round(d / t * 100))
    {
        nb_bar = 25;
        progress_bar_adv = round(d / t * nb_bar);

        std::cout << "\r ";
        SetConsoleTextAttribute(hConsole, 160);
        std::cout << " Progress : [ ";

        if (round(d / t * 100) < 10)
        {
            std::cout << "0" << round(d / t * 100) << " %]";
        }
        else
        {
            std::cout << round(d / t * 100) << " %] ";
        }

        SetConsoleTextAttribute(hConsole, 15);
        std::cout << " [";
        SetConsoleTextAttribute(hConsole, 10);
        for (int i = 0; i <= progress_bar_adv; i++)
        {
            std::cout << "#";
        }
        SetConsoleTextAttribute(hConsole, 15);
        for (int i = 0; i < nb_bar - progress_bar_adv; i++)
        {
            std::cout << ".";
        }

        std::cout << "]";
        last_progress = round(d / t * 100);
    }
    return 0;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

boolean downloadURL(const char* url, const char* fileName) {
    CURL* curl_download;
    CURLcode res;
    FILE* fp;

    curl_download = curl_easy_init();

    if (!curl_download) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return false;
    }

    if (curl_download) {
        fopen_s(&fp, fileName, "wb");

        if (!fp) {
            std::cerr << "Failed to open file: " << fileName << std::endl;
            curl_easy_cleanup(curl_download);
            return false;
        }

        curl_easy_setopt(curl_download, CURLOPT_FOLLOWLOCATION, TRUE);
        curl_easy_setopt(curl_download, CURLOPT_URL, url);
        curl_easy_setopt(curl_download, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl_download, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl_download, CURLOPT_NOPROGRESS, FALSE);
        curl_easy_setopt(curl_download, CURLOPT_PROGRESSFUNCTION, progress_bar);

        //Text color :   SetConsoleTextAttribute(hConsole, nb_color);
        SetConsoleTextAttribute(hConsole, 11);
        std::cout << " Start download of " << fileName << std::endl << std::endl;

        res = curl_easy_perform(curl_download);

        fclose(fp);

        if (res != CURLE_OK) {
            SetConsoleTextAttribute(hConsole, 4);
            std::cerr << "Download error: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl_download);
            return false;
        }

        SetConsoleTextAttribute(hConsole, 10);
        std::cout << "\nDownload completed: " << fileName << std::endl;
        curl_easy_cleanup(curl_download);
        return true;
    }
}

std::optional<fs::path> findExeFile(const fs::path& rootDir, const std::string& exeName) {
    try {
        if (!fs::exists(rootDir) || !fs::is_directory(rootDir)) {
            std::cerr << "Invalid root directory: " << rootDir << std::endl;
            return std::nullopt;
        }

        for (const auto& entry : fs::recursive_directory_iterator(rootDir)) {
            if (entry.is_regular_file() && entry.path().filename() == exeName) {
                return entry.path();
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return std::nullopt;
    }

    return std::nullopt;
}

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

void createDownloadDirectory(const std::string& path) {
    std::wstring widestr = std::wstring(path.begin(), path.end());
    CreateDirectory(widestr.c_str(), nullptr);
}

void installJava() {
    const char* command = "msiexec /i \"C:\\StateInstallerTemp\\jre_installer.msi\" ADDLOCAL=FeatureMain,FeatureEnvironment,FeatureJarFileRunWith,FeatureJavaHome,FeatureOracleJavaSoft, INSTALLDIR=\"C:\\Program Files\\Java\\\"";
    system(command);
    std::cout << command << std::endl;
}

void runInstaller(const char* filePath) {
    system(filePath);
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
    runInstaller("C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe");

    //Plugin
    excuteTeamspeakPluginInstall();

    //Technic
    runInstaller("C:\\StateInstallerTemp\\TechnicLauncher.exe");

    //Clean temp dir
    deleteTempDirectory(temp_directory);

    std::cout << "If you see this, installation is finished and you can close this window." << std::endl;

    return 0;
}
