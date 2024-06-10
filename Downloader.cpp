// Downloader.cpp
#define CURL_STATICLIB

#include "Downloader.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <cmath>
#include <windows.h>
#include <mutex>
#include <vector>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int nb_bar = 25;
std::mutex console_mutex;

struct ProgressData {
    int last_progress = -1;
    std::string file_name;
    int console_line = 0;
};

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void moveCursorToLine(int line) {
    COORD coord;
    coord.X = 0;
    coord.Y = line;
    SetConsoleCursorPosition(hConsole, coord);
}

int progress_bar(void* ptr, double total_to_download, double now_downloaded, double total_to_upload, double now_uploaded) {
    if (total_to_download == 0) return 0;  // Avoid division by zero

    ProgressData* progress_data = static_cast<ProgressData*>(ptr);
    int progress = static_cast<int>(round((now_downloaded / total_to_download) * 100));
    if (progress_data->last_progress != progress) {
        progress_data->last_progress = progress;
        int progress_bar_adv = static_cast<int>(round((now_downloaded / total_to_download) * nb_bar));

        std::lock_guard<std::mutex> lock(console_mutex);
        moveCursorToLine(progress_data->console_line);

        SetConsoleTextAttribute(hConsole, 15); // Reset to default color
        std::cout << " Progress for " << progress_data->file_name << ": [ ";
        SetConsoleTextAttribute(hConsole, 15);

        if (progress < 10) {
            std::cout << "0" << progress << " %";
        }
        else {
            std::cout << progress << " %";
        }
        std::cout << " ] [";
        SetConsoleTextAttribute(hConsole, 10); // Progress bar color
        for (int i = 0; i < progress_bar_adv; ++i) {
            std::cout << "#";
        }
        SetConsoleTextAttribute(hConsole, 15); // Reset to default color
        for (int i = progress_bar_adv; i < nb_bar; ++i) {
            std::cout << ".";
        }
        std::cout << "] ";

        std::flush(std::cout);
    }
    return 0;
}

bool downloadURL(const char* url, const char* fileName, int console_line) {
    CURL* curl_download = curl_easy_init();
    if (!curl_download) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return false;
    }

    FILE* fp = nullptr;
    if (fopen_s(&fp, fileName, "wb") != 0 || !fp) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        curl_easy_cleanup(curl_download);
        return false;
    }

    ProgressData progress_data;
    progress_data.file_name = fileName;
    progress_data.console_line = console_line;

    curl_easy_setopt(curl_download, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_download, CURLOPT_URL, url);
    curl_easy_setopt(curl_download, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl_download, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl_download, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl_download, CURLOPT_PROGRESSFUNCTION, progress_bar);
    curl_easy_setopt(curl_download, CURLOPT_PROGRESSDATA, &progress_data);

    {
        std::lock_guard<std::mutex> lock(console_mutex);
        moveCursorToLine(console_line - 1);
        SetConsoleTextAttribute(hConsole, 11);
        std::cout << "Start download of " << fileName << std::endl;
    }

    CURLcode res = curl_easy_perform(curl_download);
    fclose(fp);

    {
        std::lock_guard<std::mutex> lock(console_mutex);
        moveCursorToLine(console_line);
        if (res != CURLE_OK) {
            SetConsoleTextAttribute(hConsole, 4);
            std::cerr << "Download error: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "\nDownload completed: " << fileName << std::endl;
        }
    }

    curl_easy_cleanup(curl_download);
    return res == CURLE_OK;
}
