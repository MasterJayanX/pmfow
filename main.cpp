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
        // Windows 2000 or later
        if(string(argv[1]) == "install" || string(argv[1]) == "i"){
            // Install package
            int success = checkFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                install(argv, argc);
            }
        }
        else if(string(argv[1]) == "update" || string(argv[1]) == "u"){
            // Update repositories
            int success = checkFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                update();
            }
        }
        else if(string(argv[1]) == "list" || string(argv[1]) == "l"){
            // List packages
            int success = checkSearchFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                list();
            }
        }
        else if(string(argv[1]) == "search" || string(argv[1]) == "s"){
            // Search for packages
            int success = checkSearchFlags(argc, argv);
            if(success == 0){
                return 0;
            }
            else{
                search(argv, argc);
            }
        }
        else if(string(argv[1]) == "help" || string(argv[1]) == "h"){
            // Show help
            help();
        }
        else if(string(argv[1]) == "version" || string(argv[1]) == "v" || string(argv[1]) == "about" || string(argv[1]) == "a"){
            // Show the about section
            about(majorVersion, minorVersion, build);
        }
        else{
            cout << "Invalid command. Usage: pmfow <command>. Use pmfow help to see what commands are supported.\n";
        }
    }
    else if(argc == 1){
        if(osv.dwMajorVersion >= 5){
            // Windows 2000 or later (command not specified)
            cerr << "Error: no command specified. Usage: pmfow <command>. Use pmfow help to see what commands are supported.\n";
        }
        else{
            // Windows 98 or earlier (command not specified)
            cerr << "You're using Windows 98/ME or earlier, so your OS is not supported.\n";
        }
        return 1;
    }
    else{
        // Windows 98 or earlier
        cout << "You're using Windows 98/ME or earlier, so your OS is not supported.\n";
        return 1;
    }
    return 0;
}