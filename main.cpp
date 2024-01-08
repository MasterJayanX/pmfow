#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>
#include <winver.h>
#include <functional>
#include "commands.cpp"

using namespace std;

int main(int argc, char** argv){
    OSVERSIONINFOW osv;
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    if (GetVersionExW(&osv)){
        cout << "Windows Version: "
            << osv.dwMajorVersion << "." 
            << osv.dwMinorVersion << "." 
            << osv.dwBuildNumber << "\n";
    }
    else{
        cout << "Error getting Windows version.\n";
    }
    int majorVersion = osv.dwMajorVersion;
    int minorVersion = osv.dwMinorVersion;
    winver = getWindowsVersion(majorVersion, minorVersion);
    architecture = getArchitecture();
    if(osv.dwMajorVersion > 5 || (osv.dwMajorVersion == 5 && osv.dwMinorVersion >= 1)){
        // Windows XP or later
        if(string(argv[1]) == "install"){
            int success = checkFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                install(argv, argc);
            }
        }
        else if(string(argv[1]) == "update"){
            int success = checkFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                update();
            }
        }
        else if(string(argv[1]) == "search"){
            search(argv, argc);
        }
        else if(string(argv[1]) == "help"){
            help();
        }
        else if(string(argv[1]) == "version"){
            version();
        }
        else{
            cout << "Invalid command.\n";
        }
    }
    else{
        // Windows 2000 or earlier
        cout << "You're using Windows 2000 or earlier, so your OS is not supported.\n";
        cout << "Press Enter to exit.\n";
        cin.get();
    }
    return 0;
}