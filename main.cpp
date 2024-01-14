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
    osv.dwMajorVersion = 0, osv.dwMinorVersion = 0, osv.dwBuildNumber = 0; 
    GetVersionExW(&osv);
    int majorVersion = osv.dwMajorVersion, minorVersion = osv.dwMinorVersion, build = osv.dwBuildNumber;
    winver = getWindowsVersion(majorVersion, minorVersion);
    architecture = getArchitecture();
    programpath = getEXEpath();
    if(osv.dwMajorVersion >= 5){
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
        else if(string(argv[1]) == "list"){
            int success = checkSearchFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                list();
            }
        }
        else if(string(argv[1]) == "search"){
            int success = checkSearchFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                search(argv, argc);
            }
        }
        else if(string(argv[1]) == "help"){
            help();
        }
        else if(string(argv[1]) == "version"){
            version(majorVersion, minorVersion, build);
        }
        else{
            cout << "Invalid command.\n";
        }
    }
    else{
        // Windows 2000 or earlier
        cout << "You're using Windows 98/ME or earlier, so your OS is not supported.\n";
        cout << "Press Enter to exit.\n";
        cin.get();
    }
    return 0;
}