#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <vector>

struct DownloadItem {
    const char* url;
    const char* fileName;
};

bool downloadURL(const char* url, const char* fileName);
int progress_bar(void* bar, double t, double d);

#endif /* DOWNLOADER_H */