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
        string fullpath = programpath + "\\";
        if(os == "Windows 2000"){
            file.open(fullpath + "win2000.txt");
        }
        else if(os == "Windows XP" || os == "Windows XP Professional x64/Windows Server 2003"){
            file.open(fullpath + "winxp.txt");
        }
        else if(os == "Windows Vista"){
            file.open(fullpath + "winvista.txt");
        }
        else if(os == "Windows 7"){
            file.open(fullpath + "win7.txt");
        }
        else if(os == "Windows 8" || os == "Windows 8.1"){
            file.open(fullpath + "win8.txt");
        }
        else if(os == "Windows 10"){
            file.open(fullpath + "win10.txt");
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
    // This function returns the Windows version you are running
    string winver;
    if(majorVersion == 5 && minorVersion == 0){
        winver = "Windows 2000";
    }
    else if(majorVersion == 5 && (minorVersion == 1)){
        winver = "Windows XP";
    }
    else if(majorVersion == 5 && (minorVersion == 2)){
        winver = "Windows XP Professional x64/Windows Server 2003";
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
    // This function returns the architecture of your Windows installation
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

string getEXEpath(){
    // This function returns the path of pmfow's executable
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    return string(buffer).substr(0, pos);
}

void install(char** argv, int argc){
    // This function installs a package
    if(argc < 3){
        cout << "Usage: pmfow install <package>\n";
        return;
    }
    else{
        cout << "Installing " << argv[2] << "...\n";
        repo r(winver);
        if(r.repos(argv[2]) == "Package not found"){
            cout << "Package not found.\n";
            return;
        }
        string url = r.repos(argv[2]);
        string packagename = string(argv[2]);
        cout << url << endl;
        installPackage(packagename, url);
        cout << "Done.\n";
    }
}

void update(){
    // This function updates the repositories
    cout << "Updating repositories...\n";
    updateRepositories();
    cout << "Done.\n";
}

void list(){
    // This function lists all the packages
    cout << "Listing all packages...\n";
    Sleep(1000);
    ifstream file;
    string fullpath = programpath + "\\";
    if(winver == "Windows 2000"){
        file.open(fullpath + "win2000.txt");
    }
    else if(winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003"){
        file.open(fullpath + "winxp.txt");
    }
    else if(winver == "Windows Vista"){
        file.open(fullpath + "winvista.txt");
    }
    else if(winver == "Windows 7"){
        file.open(fullpath + "win7.txt");
    }
    else if(winver == "Windows 8" || winver == "Windows 8.1"){
        file.open(fullpath + "win8.txt");
    }
    else if(winver == "Windows 10"){
        file.open(fullpath + "win10.txt");
    }
    else{
        cout << "Invalid OS.\n";
        return;
    }
    if (!file.is_open()) {
        cerr << "Repository not found" << endl;
        return;
    }
    string line;
    int i = 0;
    while (getline(file, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != string::npos) {
            string key = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);
            cout << key << endl;
            i++;
        }
        Sleep(10);
    }
    cout << "Total packages: " << i << endl;
    file.close();
}

void search(char** argv, int argc){
    // This function searches for a package
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
            if(show_url){
                cout << "URL: " << r.repos(argv[2]) << endl;
            }
        }
    }
}

void help(){
    // This function shows the help message
    cout << "Command list:\n";
    cout << "pmfow install <package> - Installs a package.\n";
    cout << "pmfow update - Updates the repositories.\n";
    cout << "pmfow search <package> - Searches for a package.\n";
    cout << "pmfow list - Lists all the packages.\n";
    cout << "pmfow help - Shows this help message.\n";
    cout << "pmfow version - Shows the version of pmfow that you are running.\n";
    cout << "pmfow install <package> --force-os <os> - Installs a package for a different OS.\n";
    cout << "pmfow install <package> -p/--powershell - Installs a package using PowerShell's DownloadFile function.\n";
    cout << "pmfow install <package> -w/--wget - Installs a package using wget (not needed in most cases).\n";
    cout << "pmfow install <package> --check-certificates / pmfow update --check-certificates - Installs a package using wget with certificate checking.\n";
    cout << "pmfow install <package> --show-url - Shows the URL of the package.\n";
    cout << "pmfow install <package> --wget-version <os> - Installs a package using a different version of wget.\n";
    cout << "pmfow update <package> -o/--one-file - Only downloads the repository file that corresponds to the user's Windows version. It can be used alongside --force-os.\n";
    cout << "pmfow search <package> --force-os <os> - Searches for a package for a different OS.\n";
    cout << "pmfow search <package> -u/--show-url - Shows the URL of the package.\n";
}

void version(int majorVersion, int minorVersion, int build){
    // This function shows the version of pmfow that you are running
    cout << "Package Manager for Old Windows v0.1.4 (2024-01-17)" << endl;
    cout << "Made by MasterJayanX" << endl;
    cout << "Windows Version: " << winver << " (" << majorVersion << "." << minorVersion << "." << build << ")" << endl;
    cout << "Architecture: " << architecture << endl;
}

int checkFlags(int argc, char** argv){
    // This function checks the flags
    int success = 1;
    if(argc >= 3){
        for(int i = 2; i < argc; i++){
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
                    cout << "Usage: pmfow install <package> --force-os <os> / pmfow update --force-os <os>\n";
                    cout << "Valid options: 2000, xp, vista, 7, 8, 8.1, 10\n";
                    success = 0;
                    return success;
                }
                else{
                    if(string(argv[i+1]) == "2000" || string(argv[i+1]) == "Win2000" || string(argv[i+1]) == "win2000"){
                        winver = "Windows 2000";
                    }
                    else if(string(argv[i+1]) == "xp" || string(argv[i+1]) == "XP" || string(argv[i+1]) == "WinXP" || string(argv[i+1]) == "winxp"){
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
            if(string(argv[i]) == "-u" || string(argv[i]) == "--show-url"){
                show_url = true;
            }
            if(string(argv[i]) == "-o" || string(argv[i]) == "--os-file"){
                onefile = true;
            }
            if(string(argv[i]) == "--wget-version"){
                if(argc < i+1){
                    cout << "Usage: pmfow install --wget-version <os>\n";
                    cout << "Valid options: 2000, xp\n";
                    success = 0;
                    return success;
                }
                else{
                    if(string(argv[i+1]) == "2000" || string(argv[i+1]) == "Win2000" || string(argv[i+1]) == "win2000"){
                        wget_os = 5.0;
                    }
                    else if(string(argv[i+1]) == "xp" || string(argv[i+1]) == "XP" || string(argv[i+1]) == "WinXP" || string(argv[i+1]) == "winxp"){
                        wget_os = 5.1;
                    }
                    else{
                        cout << "Usage: pmfow install --wget-version <os>\n";
                        cout << "Valid options: 2000, xp\n";
                        success = 0;
                        return success;
                    }
                
                }
            }
        }
    }
    return success;
}

int checkSearchFlags(int argc, char** argv){
    // This function checks the flags for the search command
    int success = 1;
    if(argc >= 3){
        for(int i = 2; i < argc; i++){
            if(string(argv[i]) == "--force-os"){
                if(argc < i+1){
                    cout << "Usage: pmfow search <package> --force-os <os>\n";
                    cout << "Valid options: 2000, xp, vista, 7, 8, 8.1, 10\n";
                    success = 0;
                    return success;
                }
                else{
                    if(string(argv[i+1]) == "2000" || string(argv[i+1]) == "Win2000" || string(argv[i+1]) == "win2000"){
                        winver = "Windows 2000";
                    }
                    else if(string(argv[i+1]) == "xp" || string(argv[i+1]) == "XP" || string(argv[i+1]) == "WinXP" || string(argv[i+1]) == "winxp"){
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
            if(string(argv[i]) == "-u" || string(argv[i]) == "--show-url"){
                show_url = true;
            }
        }
    }
    return success;
}