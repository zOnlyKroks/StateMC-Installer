#ifndef TECHNIC_HELPER_H
#define TECHNIC_HELPER_H

#include <string>
#include <optional>

void executeTechnicLauncher(const std::string& path);
bool copyTechnicLauncher(const std::string& destination);
std::optional<std::string> getDesktopPath();
bool createNeccesaryDirectories();
void handleCreateInstalledModpackFile(const std::string& dotTechnicPath, std::ofstream& outFile);

#endif /* TECHNIC_HELPER_H */