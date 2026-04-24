#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>
#include <wininet.h>
#include <vector>
#include "config.hpp"

#pragma comment(lib, "wininet.lib")

using namespace std;

bool checkNet(){
    // Check if the user has an internet connection
    DWORD flags;
    if (InternetGetConnectedState(&flags, 0)) {
        return true;
    } else {
        return false;
    }
}

bool fileDiffs(string file1, string file2){
    // Compare two files
    ifstream f1(file1, ios::binary | ios::ate);
    ifstream f2(file2, ios::binary | ios::ate);

    if (!f1.is_open() || !f2.is_open()) {
        cerr << "Error opening files for comparison." << endl;
        return true; // Assume they differ if we can't open them
    }

    if (f1.tellg() != f2.tellg()) {
        return true; // Files differ in size
    }

    f1.seekg(0);
    f2.seekg(0);

    const size_t bufferSize = 4096;
    char buffer1[bufferSize];
    char buffer2[bufferSize];

    while (f1 && f2) {
        f1.read(buffer1, bufferSize);
        f2.read(buffer2, bufferSize);

        if (memcmp(buffer1, buffer2, f1.gcount()) != 0) {
            return true; // Files differ in content
        }
    }

    return false; // Files are the same
}

string getExtension(string url){
    // Get extension from URL
    string extension, arfterslash;
    int pos = url.find_last_of(".");
    extension = url.substr(pos);
    int pos2 = extension.find("/");
    if(pos2 != -1){
        arfterslash = extension.substr(0, pos2);
        extension = arfterslash;
    }
    return extension;
}

wstring string_to_wstring(const string& str) {
    // Convert string to wstring
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    return wstr;
}

void installPackage(string package, string url, string silentinst){
    // Install package
    string command, filename, extension, wget_exe = "wget", fullpath = programpath + "\\files\\";
    system(command.c_str());
    extension = getExtension(url);
    filename = package + extension;
    fullpath += filename;
    if(wget_os == 5.1 || ((winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003") && wget_os == 0)){
        wget_exe = "wget_xp";
    }
    else{
        wget_exe = "wget";
    }
    if(check_cert){
        command = wget_exe + " -O \"" + fullpath + "\" " + url + quiet_cmd;
    }
    else{
        command = wget_exe + " -O \"" + fullpath + "\" " + url + " --no-check-certificate" + quiet_cmd;
    }
    if (system(command.c_str()) != 0) {
        cerr << "Error downloading file: " << strerror(errno) << endl;
        log("Error downloading file " + fullpath);
        inst_success = false;
        return;
    }
    if (runasexe || (extension != ".zip" && extension != ".7z" && extension != ".rar")) {
        // Run the file
        SHELLEXECUTEINFOW ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        wstring wFullPath = string_to_wstring(fullpath);
        ShExecInfo.lpFile = wFullPath.c_str();
        if(silent){
            wstring wSilentInst = string_to_wstring(silentinst);
            ShExecInfo.lpParameters = wSilentInst.c_str();
        }
        ShExecInfo.nShow = SW_SHOWNORMAL;

        if (ShellExecuteExW(&ShExecInfo)) {
            WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
            CloseHandle(ShExecInfo.hProcess);

            // Now, the file is closed, and it can be deleted.
            if (remove(fullpath.c_str()) != 0) {
                cerr << "Error deleting file: " << strerror(errno) << endl;
                log("Error deleting file " + fullpath);
            }
        } else {
            cerr << "Error executing command: " << GetLastError() << endl;
            log("Error executing command");
        }
    }
    else{
        // Extract the file
        cout << "Please install " << filename << " manually. You can find the file at " << fullpath << endl;
        log(filename + " has to be installed manually. You can find the file at " + fullpath);
    }
}

vector<string> repoDirectories(){
    // Get directories from directories.txt
    vector<string> directories(2);
    ifstream file;
    string fullpath = programpath + "\\files\\";
    file.open(fullpath + "directories.txt");
    if(file.is_open()){
        for(int i = 0; i < 2; i++){
            getline(file, directories[i]);
        }
        file.close();
    }
    else{
        cout << "Warning: directories.txt was not found. The default directories will be used." << endl;
        log("Warning: directories.txt was not found. The default directories will be used.");
        directories[0] = "64%20bit";
        directories[1] = "32%20bit";
    }
    return directories;
}

void updateRepositories(string link){
    // Update app catalogs
    string command, file_winver, wget_exe = "wget", fullpath = programpath + "\\files\\", pmfowpath = programpath + "\\";
    if(winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003" || winver == "ReactOS"){
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
    else{
        cout << "Error: your Windows version not supported." << endl;
        log("Error: your Windows version not supported.");
        return;
    }
    if(wget_os == 5.1 || winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003" || winver == "ReactOS"){
        wget_exe = "wget_xp";
    }
    else{
        wget_exe = "wget";
    }
    vector<string> directories = repoDirectories();
    string certFlag = (check_cert) ? "" : " --no-check-certificate";
    string architectureFolder = (architecture == "x64") ? directories[0] : directories[1];
    string versionFile = (architectureFolder + "/" + file_winver + ".dat");

    bool internet = checkNet();
    if(!internet && !ignore_internet_connection){
        cout << "Error: no internet connection. Please check your connection and try again." << endl;
        log("Error: no internet connection. Please check your connection and try again.");
        return;
    }

    // Checks if the user wants to update only one file
    if (onefile) {
        command = "del \"" + fullpath + file_winver + ".dat\"";
        system(command.c_str());
        command = wget_exe + " -O \"" + fullpath + file_winver + ".dat\"" + " https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + versionFile + certFlag + quiet_cmd;
        if (system(command.c_str()) != 0) {
            cerr << "Error downloading file: " << strerror(errno) << endl;
            log("Error downloading file " + fullpath);
            return;
        }
    } else {
        bool needUpdate = false;

        command = wget_exe + " -O \"" + fullpath + "directories.txt.temp\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/directories.txt" + certFlag + quiet_cmd;
        log("Downloading directories.txt");
        system(command.c_str());
        needUpdate = fileDiffs(fullpath + "directories.txt", fullpath + "directories.txt.temp");
        if (needUpdate) {
            log("directories.txt has been updated. Updating app catalogs...");
            command = "del \"" + fullpath + "directories.txt\"";
            system(command.c_str());
            command = "ren \"" + fullpath + "directories.txt.temp\" directories.txt";
            system(command.c_str());
        }
        else{
            log("directories.txt is up to date.");
            command = "del \"" + fullpath + "directories.txt.temp\"";
            system(command.c_str());
        }

        command = wget_exe + " -O \"" + fullpath + "updater.dat.temp\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/updater.dat" + certFlag + quiet_cmd;
        log("Downloading updater.dat");
        system(command.c_str());
        needUpdate = fileDiffs(fullpath + "updater.dat", fullpath + "updater.dat.temp");
        if (needUpdate) {
            log("updater.dat has been updated. Updating pmfow-updater.exe...");
            command = "del \"" + fullpath + "updater.dat\"";
            system(command.c_str());
            command = "ren \"" + fullpath + "updater.dat.temp\" updater.dat";
            system(command.c_str());
        }
        else{
            log("updater.dat is up to date.");
            command = "del \"" + fullpath + "updater.dat.temp\"";
            system(command.c_str());
        }

        command = wget_exe + " -O \"" + fullpath + "uninstallers.dat\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/uninstallers.dat" + certFlag + quiet_cmd;
        log("Downloading uninstallers.dat");
        system(command.c_str());
        needUpdate = fileDiffs(fullpath + "uninstallers.dat", fullpath + "uninstallers.dat.temp");
        if (needUpdate) {
            log("uninstallers.dat has been updated. Updating uninstallers...");
            command = "del \"" + fullpath + "uninstallers.dat\"";
            system(command.c_str());
            command = "ren \"" + fullpath + "uninstallers.dat.temp\" uninstallers.dat";
            system(command.c_str());
        }
        else{
            log("uninstallers.dat is up to date.");
            command = "del \"" + fullpath + "uninstallers.dat.temp\"";
            system(command.c_str());
        }

        command = wget_exe + " -O \"" + pmfowpath + "pmfow-updater.exe.temp\" " + link + certFlag + quiet_cmd;
        log("Downloading pmfow-updater.exe");
        system(command.c_str());
        needUpdate = fileDiffs(pmfowpath + "pmfow-updater.exe", fullpath + "pmfow-updater.exe.temp");
        if (needUpdate) {
            log("pmfow-updater.exe has been updated. Updating pmfow-updater.exe...");
            command = "del \"" + pmfowpath + "pmfow-updater.exe\"";
            system(command.c_str());
            command = "ren \"" + pmfowpath + "pmfow-updater.exe.temp\" pmfow-updater.exe";
            system(command.c_str());
        }
        else{
            log("pmfow-updater.exe is up to date.");
            command = "del \"" + pmfowpath + "pmfow-updater.exe.temp\"";
            system(command.c_str());
        }
        
        if(upd_config){
            command = "del \"" + fullpath + "config.ini\"";
            system(command.c_str());
            command = wget_exe + " -O \"" + fullpath + "config.ini\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/config.ini" + certFlag + quiet_cmd;
            log("Downloading config.ini");
            system(command.c_str());
        }
        
        log("Updating app catalogs...");
        vector<string> versions = {"winxp", "winvista", "win7", "win8", "win10"};
        for (const auto& version : versions) {
            command = "del \"" + fullpath + version + ".dat\"";
            system(command.c_str());
            command = wget_exe + " -O \"" + fullpath + version + ".dat\"" + " https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + architectureFolder + "/" + version + ".dat" + certFlag + quiet_cmd;
            log("Downloading " + version + ".dat");
            system(command.c_str());
        }
    }
}