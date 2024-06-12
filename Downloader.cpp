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
const int kProgressBarWidth = 25;
std::mutex console_mutex;

struct ProgressData {
    int last_progress = -1;
    std::string file_name;
    int console_line = 0;
};

size_t WriteData(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void moveCursorToLine(int line) {
    COORD coord;
    coord.X = 0;
    coord.Y = line;
    SetConsoleCursorPosition(hConsole, coord);
}

int ProgressCallback(void* ptr, double total_to_download, double now_downloaded, double, double) {
    if (total_to_download == 0) return 0;

    ProgressData* progress_data = static_cast<ProgressData*>(ptr);
    int progress = static_cast<int>(round((now_downloaded / total_to_download) * 100));
    if (progress_data->last_progress != progress) {
        progress_data->last_progress = progress;
        int progress_bar_width = static_cast<int>(round((now_downloaded / total_to_download) * kProgressBarWidth));

        std::lock_guard<std::mutex> lock(console_mutex);
        moveCursorToLine(progress_data->console_line);

        SetConsoleTextAttribute(hConsole, 15);
        std::cout << " Progress for " << progress_data->file_name << ": [ ";
        std::cout << (progress < 10 ? "0" : "") << progress << " % ] [";

        SetConsoleTextAttribute(hConsole, 10);
        for (int i = 0; i < progress_bar_width; ++i) {
            std::cout << "#";
        }
        SetConsoleTextAttribute(hConsole, 15);
        for (int i = progress_bar_width; i < kProgressBarWidth; ++i) {
            std::cout << ".";
        }
        std::cout << "] ";

        std::flush(std::cout);
    }
    return 0;
}

bool downloadURL(const char* url, const char* file_name, int console_line) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return false;
    }

    FILE* file = nullptr;
    if (fopen_s(&file, file_name, "wb") != 0 || !file) {
        std::cerr << "Failed to open file: " << file_name << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    ProgressData progress_data;
    progress_data.file_name = file_name;
    progress_data.console_line = console_line;

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &progress_data);

    {
        std::lock_guard<std::mutex> lock(console_mutex);
        moveCursorToLine(console_line - 1);
        SetConsoleTextAttribute(hConsole, 11);
        std::cout << "Start download of " << file_name << std::endl;
    }

    CURLcode res = curl_easy_perform(curl);
    fclose(file);

    {
        std::lock_guard<std::mutex> lock(console_mutex);
        moveCursorToLine(console_line);
        if (res != CURLE_OK) {
            SetConsoleTextAttribute(hConsole, 4);
            std::cerr << "Download error: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            SetConsoleTextAttribute(hConsole, 10);
            std::cout << "\nDownload completed: " << file_name << std::endl;
        }
    }

    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}
