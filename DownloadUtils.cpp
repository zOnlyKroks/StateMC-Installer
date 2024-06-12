#include "DownloadUtils.h"

#include <thread>
#include <iostream>
#include <vector>
#include "Downloader.h"
#include "algorithm"

bool downloadFilesConcurrently(const std::vector<std::pair<const char*, const char*>>& downloadUrls) {
    std::vector<std::thread> downloadThreads;
    std::vector<bool> downloadResults(downloadUrls.size(), false);  
    int consoleLine = 3; 

    for (size_t i = 0; i < downloadUrls.size(); ++i) {
        const auto& [url, fileName] = downloadUrls[i];
        downloadThreads.emplace_back([url, fileName, consoleLine, &downloadResults, i]() {
            downloadResults[i] = downloadURL(url, fileName, consoleLine);
            if (!downloadResults[i]) {
                std::cerr << "Failed to download: " << fileName << std::endl;
            }
            });
        consoleLine += 4;
    }

    for (auto& thread : downloadThreads) {
        thread.join();
    }

    bool allDownloadsSuccessful = std::all_of(downloadResults.begin(), downloadResults.end(), [](bool result) { return result; });

    return allDownloadsSuccessful;
}
