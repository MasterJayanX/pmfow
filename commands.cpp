#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <windows.h>
#include <winver.h>
#include <functional>
#include "downloader.cpp"

using namespace std;

class repo {
public:
    repo(const string& os) {
        // This constructor calls the loadRepos function
        loadRepos(os);
    }

    string repos(const string& key) const {
        auto it = packages.find(key);
        return (it != packages.end()) ? it->second : "Package not found";
    }

private:
    // This map contains the packages for each repo
    map<string, string> packages;

    void loadRepos(const string& os) {
        ifstream file;
        if(os == "Windows XP"){
            file.open("winxp.txt");
        }
        else if(os == "Windows Vista"){
            file.open("winvista.txt");
        }
        else if(os == "Windows 7"){
            file.open("win7.txt");
        }
        else if(os == "Windows 8" || os == "Windows 8.1"){
            file.open("win8.txt");
        }
        else if(os == "Windows 10"){
            file.open("win10.txt");
        }
        else{
            cout << "Invalid OS.\n";
        }
        if (!file.is_open()) {
            cerr << "Repository not found" << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != string::npos) {
                string key = line.substr(0, delimiterPos);
                string value = line.substr(delimiterPos + 1);
                packages[key] = value;
            }
        }
        file.close();
    }
};

string getWindowsVersion(int majorVersion, int minorVersion){
    string winver;
    if(majorVersion == 5 && minorVersion == 1){
        winver = "Windows XP";
    }
    else if(majorVersion == 6 && minorVersion == 0){
        winver = "Windows Vista";
    }
    else if(majorVersion == 6 && minorVersion == 1){
        winver = "Windows 7";
    }
    else if(majorVersion == 6 && minorVersion == 2){
        winver = "Windows 8";
    }
    else if(majorVersion == 6 && minorVersion == 3){
        winver = "Windows 8.1";
    }
    else if(majorVersion == 10 && minorVersion == 0){
        winver = "Windows 10";
    }
    else{
        winver = "Other Windows version";
    }
    return winver;
}

string getArchitecture(){
    string arch;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    switch(sysInfo.wProcessorArchitecture){
        case PROCESSOR_ARCHITECTURE_AMD64:
            arch = "x64";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            arch = "arm64";
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            arch = "itanium";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            arch = "x86";
            break;
        case PROCESSOR_ARCHITECTURE_UNKNOWN:
            arch = "Unknown";
            break;
    }
    return arch;
}

void install(char** argv, int argc){
    if(argc < 3){
        cout << "Usage: pmfow install <package>\n";
        return;
    }
    else{
        cout << "Installing " << argv[2] << "...\n";
        repo r(winver);
        string url = r.repos(argv[2]);
        string packagename = string(argv[2]);
        cout << url << endl;
        installPackage(packagename, url);
        cout << "Done.\n";
    }
}

void update(){
    cout << "Updating repositories...\n";
    updateRepositories();
    cout << "Done.\n";
}

void search(char** argv, int argc){
    if(argc < 3){
        cout << "Usage: pmfow search <package>\n";
        return;
    }
    else{
        cout << "Searching for " << argv[2] << "...\n";
        repo r(winver);
        if(r.repos(argv[2]) == "Package not found"){
            cout << "Package not found.\n";
        }
        else{
            cout << "Package found: " << argv[2] << endl;
        }
    }
}

void help(){
    cout << "Command list:\n";
    cout << "pmfow install <package> - Installs a package\n";
    cout << "pmfow update - Updates the repositories\n";
    cout << "pmfow search <package> - Searches for a package\n";
    cout << "pmfow help - Shows this help message\n";
    cout << "pmfow version - Shows the version of pmfow that you are running\n";
    cout << "pmfow install <package> --force-os <os> - Installs a package for a different OS\n";
    cout << "pmfow install <package> --powershell - Installs a package using PowerShell (not needed for Windows 7 or above)\n";
    cout << "pmfow install <package> --wget - Installs a package using wget (not needed for Windows XP or Vista)\n";
    cout << "pmfow install <package> --check-certificates - Installs a package using wget with certificate checking\n";
}

void version(){
    cout << "Package Manager for Old Windows v0.1.0" << endl;
    cout << "Made by MasterJayanX" << endl;
    cout << "Windows Version: " << winver << endl;
    cout << "Architecture: " << architecture << endl;
}

int checkFlags(int argc, char** argv){
    int success = 1;
    if(argc > 3){
        for(int i = 3; i < argc; i++){
            if(string(argv[i]) == "-p" || string(argv[i]) == "--powershell"){
                use_powershell = true;
            }
            else if(string(argv[i]) == "-w" || string(argv[i]) == "--wget"){
                use_powershell = false;
            }
            if(string(argv[i]) == "-c" || string(argv[i]) == "--check-certificates"){
                if(use_powershell){
                    cout << "PowerShell doesn't support the --check-certificates flag.\n";
                    cout << "Please use this alongside the --wget flag instead.\n";
                    success = 0;
                    return success;
                }
                else{
                    check_cert = true;
                }
            }
            if(string(argv[i]) == "--force-os"){
                if(argc < i+1){
                    cout << "Usage: pmfow install <package> --force-os <os>\n";
                    cout << "Valid options: xp, vista, 7, 8, 8.1, 10\n";
                    success = 0;
                    return success;
                }
                else{
                    if(string(argv[i+1]) == "xp" || string(argv[i+1]) == "XP" || string(argv[i+1]) == "WinXP" || string(argv[i+1]) == "winxp"){
                        winver = "Windows XP";
                    }
                    else if(string(argv[i+1]) == "vista" || string(argv[i+1]) == "Vista" || string(argv[i+1]) == "WinVista" || string(argv[i+1]) == "winvista"){
                        winver = "Windows Vista";
                    }
                    else if(string(argv[i+1]) == "7" || string(argv[i+1]) == "Win7" || string(argv[i+1]) == "win7"){
                        winver = "Windows 7";
                    }
                    else if(string(argv[i+1]) == "8" || string(argv[i+1]) == "Win8" || string(argv[i+1]) == "win8"){
                        winver = "Windows 8";
                    }
                    else if(string(argv[i+1]) == "8.1" || string(argv[i+1]) == "Win8.1" || string(argv[i+1]) == "win8.1"){
                        winver = "Windows 8.1";
                    }
                    else if(string(argv[i+1]) == "10" || string(argv[i+1]) == "Win10" || string(argv[i+1]) == "win10"){
                        winver = "Windows 10";
                    }
                    else{
                        cout << "Invalid OS.\n";
                        success = 0;
                        return success;
                    }
                }
            }
        }
    }
    return success;
}