#include "JavaHelper.h"

#include <iostream>
#include <string>
#include "ProcessHelper.h"

void installJava() {
    std::cout << "Installing Java..." << std::endl;
    const char* command = "msiexec /i \"C:\\StateInstallerTemp\\jre_installer.msi\" ADDLOCAL=FeatureMain,FeatureEnvironment,FeatureJarFileRunWith,FeatureJavaHome,FeatureOracleJavaSoft INSTALLDIR=\"C:\\Program Files\\Java\\\"";
    if (!runProcess(command)) {
        std::cerr << "Failed to install Java." << std::endl;
    }
    else {
        std::cout << "Java installed." << std::endl;
    }
}