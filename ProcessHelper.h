#ifndef PROCESS_HELPER_H
#define PROCESS_HELPER_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#pragma comment(lib, "Shell32.lib")

bool runProcess(const std::string& command);

#endif /* PROCESS_HELPER_H */