#include "DownloadUtils.h"

#include <thread>
#include <iostream>
#include "Downloader.h"

bool downloadFilesConcurrently(std::vector<std::pair<const char*, const char*>> downloadUrls) {
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