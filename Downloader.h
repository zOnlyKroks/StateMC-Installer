#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <vector>

struct DownloadItem {
    const char* url;
    const char* fileName;
};

bool downloadURL(const char* url, const char* fileName, int console_line);
void moveCursorToLine(int line);
int progress_bar(void* bar, double t, double d, double ultotal, double ulnow);

#endif /* DOWNLOADER_H */