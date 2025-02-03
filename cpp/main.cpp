#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>
#include <winver.h>
#include <functional>
// I'm too lazy to use a Makefile, sorry
#include "commands.cpp"

using namespace std;

int main(int argc, char** argv){
    OSVERSIONINFOW osv;
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    // initialize the variables for the Windows version
    osv.dwMajorVersion = 0, osv.dwMinorVersion = 0, osv.dwBuildNumber = 0;
    GetVersionExW(&osv);
    // get the Windows version
    majorVersion = osv.dwMajorVersion; 
    minorVersion = osv.dwMinorVersion;
    build = osv.dwBuildNumber;
    winver = getWindowsVersion();
    architecture = getArchitecture();
    programpath = getEXEpath();
    if(osv.dwMajorVersion >= 5 && argc > 1){
        // Windows 2000 or later
        loadConfig();
        if(string(argv[1]) == "install" || string(argv[1]) == "i"){
            // Install package
            int success = checkFlags(argc, argv);
            if(success == 0){
                log_from_main(argv, argc, "Execution ended with code 1.");
                return 1;
            }
            else{
                log_from_main(argv, argc, "no_message");
                install(argv, argc);
            }
        }
        else if(string(argv[1]) == "uninstall" || string(argv[1]) == "remove" || string(argv[1]) == "rm"){
            // Uninstall package
            int success = checkFlags(argc, argv);
            if(success == 0){
                log_from_main(argv, argc, "Execution ended with code 1.");
                return 1;
            }
            else{
                log_from_main(argv, argc, "no_message");
                uninstall(argv, argc);
            }
        }
        else if(string(argv[1]) == "update" || string(argv[1]) == "u"){
            // Update app catalogs
            int success = checkFlags(argc, argv);
            if(success == 0){
                log_from_main(argv, argc, "Execution ended with code 1.");
                return 1;
            }
            else{
                log_from_main(argv, argc, "no_message");
                update();
            }
        }
        else if(string(argv[1]) == "list" || string(argv[1]) == "l"){
            // List packages
            int success = checkFlags(argc, argv);
            if(success == 0){
                log_from_main(argv, argc, "Execution ended with code 1.");
                return 1;
            }
            else{
                if(list_uninstall){
                    listUninstall();
                }
                else{
                    log_from_main(argv, argc, "no_message");
                    list();
                }
            }
        }
        else if(string(argv[1]) == "search" || string(argv[1]) == "s"){
            // Search for packages
            int success = checkFlags(argc, argv);
            if(success == 0){
                log_from_main(argv, argc, "Execution ended with code 1.");
                return 1;
            }
            else{
                log_from_main(argv, argc, "no_message");
                search(argv, argc);
            }
        }
        else if(string(argv[1]) == "help" || string(argv[1]) == "h"){
            // Show help
            help();
        }
        else if(string(argv[1]) == "version" || string(argv[1]) == "v"){
            // Show the about section
            int success = checkFlags(argc, argv);
            if(success == 0){
                log_from_main(argv, argc, "Execution ended with code 1.");
                return 1;
            }
            else{
                log_from_main(argv, argc, "no_message");
                version();
            }
        }
        else if(string(argv[1]) == "about" || string(argv[1]) == "a"){
            int success = checkFlags(argc, argv);
            if(success == 0){
                log_from_main(argv, argc, "Execution ended with code 1.");
                return 1;
            }
            else{
                log_from_main(argv, argc, "no_message");
                about(majorVersion, minorVersion, build);
            }
        }
        else{
            cout << "Invalid command. Usage: pmfow <command>. Use pmfow help to see what commands are supported.\n";
            log_from_main(argv, argc, "Invalid command. Use pmfow help to see what commands are supported.");
        }
    }
    else if(argc == 1){
        string message;
        if(osv.dwMajorVersion >= 5){
            // Windows 2000 or later (command not specified)
            message = "Error: no command specified. Usage: pmfow <command>. Use pmfow help to see what commands are supported.";
        }
        else{
            // Windows 98 or earlier (command not specified)
            message = "You're using Windows 98/ME or earlier, so your OS is not supported. You need Windows 2000 or later to use pmfow.";
        }
        cerr << message << endl;
        log_from_main(argv, argc, message);
        log("Execution ended with code 1.");
        return 1;
    }
    else{
        // Windows 98 or earlier
        if(string(argv[1]) == "help" || string(argv[1]) == "h"){
            // Show help
            help();
        }
        else if(string(argv[1]) == "version" || string(argv[1]) == "v"){
            // Show the pmfow version
            version();
        }
        else if(string(argv[1]) == "about" || string(argv[1]) == "a"){
            // Show the about section
            about(majorVersion, minorVersion, build);
        }
        else{
            // Other commands are not supported in these old versions
            string message = "You're using Windows 98/ME or earlier, so your OS is not supported. You need Windows 2000 or later to use pmfow.";
            cout << message << endl;
            log_from_main(argv, argc, message);
            log("Execution ended with code 1.");
            return 1;
        }
        cin.ignore();
        cin.get();
    }
    return 0;
}