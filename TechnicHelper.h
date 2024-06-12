#ifndef TECHNIC_HELPER_H
#define TECHNIC_HELPER_H

#include <string>
#include <optional>

void moveAndExecuteTechnicLauncher();
void executeTechnicLauncher(const std::string& path);
bool copyTechnicLauncher(const std::string& destination);
std::optional<std::string> getDesktopPath();

#endif /* TECHNIC_HELPER_H */