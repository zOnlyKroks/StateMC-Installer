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

int nb_bar;
double last_progress, progress_bar_adv;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int progress_bar(void* bar, double t, double d)
{
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

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static void downloadURL(const char* url, const char* fileName) {
    CURL* curl_download;
    CURLcode res;
    FILE* fp;

    curl_download = curl_easy_init();
    if (curl_download) {
        fopen_s(&fp, fileName, "wb");

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
        if (res == CURLE_OK)
        {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << std::endl << std::endl << " The file was download with succes" << std::endl;
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 4);
            std::cout << std::endl << std::endl << " Error" << std::endl;
        }
        curl_easy_cleanup(curl_download);
    }
}

const char* output_file_jre = "C:\\StateInstallerTemp\\jre_installer.msi";
const char* output_file_technic = "C:\\StateInstallerTemp\\TechnicLauncher.exe";
const char* output_file_teamspeak = "C:\\StateInstallerTemp\\TeamSpeak_Client_v3.6.0.exe";
const char* output_file_plugin = "C:\\StateInstallerTemp\\StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin";

static void installJava() {
    downloadURL("https://api.adoptium.net/v3/installer/latest/8/ga/windows/x64/jre/hotspot/normal/eclipse?project=jdk", output_file_jre);
}

static void installTechnicLauncher() {
    downloadURL("https://launcher.technicpack.net/launcher4/831/TechnicLauncher.exe", output_file_technic);
}

static void installTeamspeakLauncher() {
    downloadURL("https://download.filepuma.com/files/chat-and-instant-messaging/teamspeak-client-64bit-/TeamSpeak_Client_(64bit)_v3.6.0.exe", output_file_teamspeak);
}

static void installTeamspeakPlugin() {
    downloadURL("https://www.statemc.de/download/ts/StateMC_Voice_Plugin-2.1.0-win64.ts3_plugin", output_file_plugin);
}

int main() {
    std::cout << "StateMC Installer" << std::endl;

    const std::string str = "C:\\StateInstallerTemp\\";
    std::wstring widestr = std::wstring(str.begin(), str.end());
    const wchar_t* widecstr = widestr.c_str();

    CreateDirectory(widecstr, NULL);


    //installJava();
    //installTechnicLauncher();
    //installTeamspeakLauncher();
    installTeamspeakPlugin();

    const char* command = "msiexec /i \"C:\\StateInstallerTemp\\jre_installer.msi\" ADDLOCAL=FeatureMain,FeatureEnvironment,FeatureJarFileRunWith,FeatureJavaHome,FeatureOracleJavaSoft, INSTALLDIR=\"C:\\Program Files\\Java\\\"";

    system(command);

    std::cout << command << std::endl;

    //Teamspeak
    system(output_file_teamspeak);

    //Technic
    system(output_file_technic);


    std::cout << "If you see this, installation is finished and you can close this window." << std::endl;

    return 0;
}
