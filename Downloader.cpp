#define CURL_STATICLIB

#include "Downloader.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <curl/curl.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int nb_bar;
double last_progress, progress_bar_adv;

bool downloadURL(const char* url, const char* fileName) {
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