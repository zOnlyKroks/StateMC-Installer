#ifndef DOWNLOAD_HELPER_H
#define DOWNLOAD_HELPER_H

#include <vector>

bool downloadFilesConcurrently(std::vector<std::pair<const char*, const char*>> downloadUrls);

#endif /* DOWNLOAD_HELPER_H */