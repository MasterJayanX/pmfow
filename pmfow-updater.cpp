#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <windows.h>
#include <winver.h>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

class repo {
public:
    repo(const string& os, const string& programpath) {
        // This constructor calls the loadRepos function
        loadRepos(os, programpath);
    }

    string repos(const string& key) const {
        auto it = packages.find(key);
        return (it != packages.end()) ? it->second : "Package not found";
    }

private:
    // This map contains the packages for each repo
    map<string, string> packages;

    void loadRepos(const string& os, const string& programpath) {
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

string getWindowsVersion(int majorVersion, int minorVersion, int buildNumber){
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

vector<string> repoDirectories(string programpath){
    vector<string> directories(2);
    ifstream file;
    string fullpath = programpath + "\\";
    file.open(fullpath + "directories.txt");
    if(file.is_open()){
        for(int i = 0; i < 2; i++){
            getline(file, directories[i]);
        }
        file.close();
    }
    else{
        cout << "Error: directories.txt was not found. The default directories will be used." << endl;
        directories[0] = "64%20bit";
        directories[1] = "32%20bit";
    }
    return directories;
}

string definewin(string winver){
    string file_winver;
    if(winver == "Windows 2000"){
        file_winver = "win2000";
    }
    if(winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003"){
        file_winver = "winxp";
    }
    else if(winver == "Windows Vista"){
        file_winver = "winvista";
    }
    else if(winver == "Windows 7"){
        file_winver = "win7";
    }
    else if(winver == "Windows 8" || winver == "Windows 8.1"){
        file_winver = "win8";
    }
    else if(winver == "Windows 10" || winver == "Windows 11"){
        file_winver = "win10";
    }
    return file_winver;
}

int main(){
    OSVERSIONINFOW osv;
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
    osv.dwMajorVersion = 0, osv.dwMinorVersion = 0, osv.dwBuildNumber = 0; 
    GetVersionExW(&osv);
    int majorVersion = osv.dwMajorVersion, minorVersion = osv.dwMinorVersion, build = osv.dwBuildNumber;
    string winver = getWindowsVersion(majorVersion, minorVersion, build), architecture = getArchitecture(), programpath = getEXEpath(), wget_exe, fullpath = programpath + "\\";
    if(winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003"){
        wget_exe = "wget_xp";
    }
    else if(winver == "Windows 2000"){
        wget_exe = "wget_2k";
    }
    else{
        wget_exe = "wget";
    }
    vector<string> directories = repoDirectories(programpath);
    string file_winver = definewin(winver);
    string architectureFolder = (architecture == "x64") ? directories[0] : directories[1];
    string versionFile = (architectureFolder + "/" + file_winver + ".txt");
    if(osv.dwMajorVersion >= 5){
        // Windows 2000 or later
        system(("del " + fullpath + file_winver + ".txt").c_str());
        system((wget_exe + " -O " + fullpath + file_winver + ".txt https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + versionFile).c_str());
        system(("del " + fullpath + "directories.txt").c_str());
        system((wget_exe + " -O " + fullpath + "directories.txt https://raw.githubusercontent.com/MasterJayanX/pmfow/main/directories.txt").c_str());
        repo r(winver, programpath);
        string package = r.repos("pmfow");
        system(("del " + fullpath + "pmfow.exe").c_str());
        system((wget_exe + " -O " + fullpath + "pmfow.exe " + package).c_str());
    }
    else{
        // Windows 98 or earlier
        cout << "Your Windows version is not supported.\n";
    }
    return 0;
}