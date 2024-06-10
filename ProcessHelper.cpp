#include "ProcessHelper.h"

bool runProcess(const std::string& command) {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Create a copy of the command string as CreateProcess modifies it
    std::vector<char> cmd(command.begin(), command.end());
    cmd.push_back('\0');

    if (!CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        // If CreateProcessA fails, check the error
        DWORD error = GetLastError();
        if (error == ERROR_ELEVATION_REQUIRED || error == ERROR_ACCESS_DENIED || error == ERROR_PRIVILEGE_NOT_HELD) {
            // If elevation is required, use ShellExecuteEx to run the command with elevated privileges
            SHELLEXECUTEINFOA sei = { sizeof(sei) };
            sei.lpVerb = "runas";  // Request elevation
            sei.lpFile = "cmd.exe";
            std::string parameters = "/C " + command;
            sei.lpParameters = parameters.c_str();
            sei.nShow = SW_SHOWNORMAL;
            sei.fMask = SEE_MASK_NOCLOSEPROCESS;

            if (!ShellExecuteExA(&sei)) {
                DWORD shellError = GetLastError();
                std::cerr << "ShellExecuteEx failed (" << shellError << "): " << command << std::endl;
                return false;
            }

            // Wait until the process exits
            WaitForSingleObject(sei.hProcess, INFINITE);
            CloseHandle(sei.hProcess);
            return true;
        }
        else {
            std::cerr << "CreateProcess failed (" << error << "): " << command << std::endl;
            return false;
        }
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}