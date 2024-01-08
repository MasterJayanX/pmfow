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
    if(winver == "Windows XP" || winver == "Windows Vista"){
        use_powershell = false;
    }
    else{
        use_powershell = true;
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
}

void version(){
    cout << "Package Manager for Old Windows v0.1.0" << endl;
    cout << "Made by MasterJayanX" << endl;
    cout << "Windows Version: " << winver << endl;
    cout << "Architecture: " << architecture << endl;
}