#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <vector>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;


void deleteTempDirectory(const std::string& path);
std::optional<fs::path> findExeFile(const fs::path& rootDir, const std::string& exeName);
void createDownloadDirectory(const std::string& path);
void finishUp();
bool createFolderIfNoExist(const std::string& path);

#endif /* FILEUTILS_H */