#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>
#include <vector>
#include "config.hpp"

using namespace std;

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
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstrTo[0], size_needed);
    return wstrTo;
}

void installPackage(string package, string url, string silentinst){
    // Install package
    string command, filename, extension, wget_exe = "wget", fullpath = programpath + "\\files\\";
    system(command.c_str());
    extension = getExtension(url);
    filename = package + extension;
    fullpath += filename;
    if(wget_os == 5.0 || (winver == "Windows 2000" && wget_os == 0)){
        wget_exe = "wget_2k";
        quiet_cmd = " -q";
    }
    else if(wget_os == 5.1 || ((winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003") && wget_os == 0)){
        wget_exe = "wget_xp";
    }
    else{
        wget_exe = "wget";
    }
    log("wget executable used: " + wget_exe + ".exe");
    if(check_cert){
        command = wget_exe + " -O \"" + fullpath + "\" " + url + quiet_cmd;
    }
    else{
        command = wget_exe + " -O \"" + fullpath + "\" " + url + " --no-check-certificate" + quiet_cmd;
    }
    if (system(command.c_str()) != 0) {
        cerr << "Error downloading file: " << strerror(errno) << endl;
        log("Error downloading file " + fullpath);
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
        cout << "Error: directories.txt was not found. The default directories will be used." << endl;
        log("Error: directories.txt was not found. The default directories will be used.");
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
    else if(winver == "Windows 2000"){
        file_winver = "win2000";
    }
    else{
        cout << "Error: your Windows version not supported." << endl;
        log("Error: your Windows version not supported.");
        return;
    }
    if(wget_os == 5.0 || winver == "Windows 2000"){
        wget_exe = "wget_2k";
        quiet_cmd = " -q";
    }
    else if(wget_os == 5.1 || winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003"){
        wget_exe = "wget_xp";
    }
    else{
        wget_exe = "wget";
    }
    vector<string> directories = repoDirectories();
    string certFlag = (check_cert) ? "" : " --no-check-certificate";
    string architectureFolder = (architecture == "x64") ? directories[0] : directories[1];
    string versionFile = (architectureFolder + "/" + file_winver + ".dat");

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
        command = "del \"" + fullpath + "directories.txt\"";
        system(command.c_str());
        command = wget_exe + " -O \"" + fullpath + "directories.txt\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/directories.txt" + certFlag + quiet_cmd;
        log("Downloading directories.txt");
        system(command.c_str());
        command = "del \"" + fullpath + "updater.dat\"";
        system(command.c_str());
        command = wget_exe + " -O \"" + fullpath + "updater.dat\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/updater.dat" + certFlag + quiet_cmd;
        log("Downloading updater.dat");
        system(command.c_str());
        command = "del \"" + fullpath + "uninstallers.dat\"";
        system(command.c_str());
        command = wget_exe + " -O \"" + fullpath + "uninstallers.dat\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/uninstallers.dat" + certFlag + quiet_cmd;
        log("Downloading uninstallers.dat");
        system(command.c_str());
        command = "del \"" + pmfowpath + "pmfow-updater.exe\"";
        system(command.c_str());
        command = wget_exe + " -O \"" + pmfowpath + "pmfow-updater.exe\" " + link + certFlag + quiet_cmd;
        log("Downloading pmfow-updater.exe");
        system(command.c_str());
        
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

    if (!onefile && architecture == "x86") {
        // Additional file for Windows 2000
        command = "del \"" + fullpath + "win2000.dat\"";
        log("Deleting " + fullpath + "win2000.dat");
        system(command.c_str());
        command = wget_exe + " -O \"" + fullpath + "win2000.dat\" https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + directories[1] + "/win2000.dat" + ((check_cert) ? "" : " --no-check-certificate") + quiet_cmd;
        log("Downloading win2000.dat");
        system(command.c_str());
    }
}