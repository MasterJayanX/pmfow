#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>
#include <vector>

using namespace std;

// Global variables
string winver, architecture, programpath;
bool check_cert = false, show_url = false, onefile = false, unstable = false;
float wget_os = 0;

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

void installPackage(string package, string url){
    // Install package
    string command, filename, extension, wget_exe = "wget", fullpath = programpath + "\\";
    system(command.c_str());
    extension = getExtension(url);
    filename = package + extension;
    fullpath += filename;
    if(show_url){
        cout << "URL: " << url << endl;
    }
    if(wget_os == 5.0 || (winver == "Windows 2000" && wget_os == 0)){
        wget_exe = "wget_2k";
    }
    else if(wget_os == 5.1 || ((winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003") && wget_os == 0)){
        wget_exe = "wget_xp";
    }
    else{
        wget_exe = "wget";
    }
    if(check_cert){
        command = wget_exe + " -O " + fullpath + " " + url;
    }
    else{
        command = wget_exe + " -O " + fullpath + " " + url + " --no-check-certificate";
    }
    system(command.c_str());
    if (extension != ".zip" && extension != ".7z") {
        SHELLEXECUTEINFO ShExecInfo = {0};
        ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExecInfo.lpFile = fullpath.c_str();
        ShExecInfo.nShow = SW_SHOWNORMAL;

        if (ShellExecuteEx(&ShExecInfo)) {
            WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
            CloseHandle(ShExecInfo.hProcess);

            // Now, the file is closed, and it can be deleted.
            if (remove(fullpath.c_str()) != 0) {
                cerr << "Error deleting file: " << strerror(errno) << endl;
            }
        } else {
            cerr << "Error executing command: " << GetLastError() << endl;
        }
    }
    else{
        cout << "Please install " << filename << " manually. You can find the file at " << programpath << endl;
    }
}

vector<string> repoDirectories(){
    // Get directories from directories.txt
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

void updateRepositories(string link){
    // Update repositories
    string command, file_winver, wget_exe = "wget", fullpath = programpath + "\\";
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
    else if(winver == "Windows 2000"){
        file_winver = "win2000";
    }
    else{
        cout << "Error: Windows version not supported." << endl;
        return;
    }
    if(wget_os == 5.0 || winver == "Windows 2000"){
        wget_exe = "wget_2k";
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

    if (onefile) {
        command = "del " + fullpath + file_winver + ".dat";
        system(command.c_str());
        command = wget_exe + " -O " + fullpath + file_winver + ".dat" + " https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + versionFile + certFlag;
        system(command.c_str());
    } else {
        command = "del " + fullpath + "directories.txt";
        system(command.c_str());
        command = wget_exe + " -O " + fullpath + "directories.txt https://raw.githubusercontent.com/MasterJayanX/pmfow/main/directories.txt" + certFlag;
        system(command.c_str());
        command = "del " + fullpath + "updater.dat";
        system(command.c_str());
        command = wget_exe + " -O " + fullpath + "updater.dat https://raw.githubusercontent.com/MasterJayanX/pmfow/main/updater.dat" + certFlag;
        system(command.c_str());
        command = "del " + fullpath + "uninstallers.dat";
        system(command.c_str());
        command = wget_exe + " -O " + fullpath + "uninstallers.dat https://raw.githubusercontent.com/MasterJayanX/pmfow/main/uninstallers.dat" + certFlag;
        system(command.c_str());
        command = "del " + fullpath + "pmfow-updater.exe";
        system(command.c_str());
        command = wget_exe + " -O " + fullpath + "pmfow-updater.exe " + link + certFlag;
        system(command.c_str());
        for (const auto& version : {"winxp", "winvista", "win7", "win8", "win10"}) {
            command = "del " + fullpath + version + ".dat";
            system(command.c_str());
            command = wget_exe + " -O " + fullpath + version + ".dat" + " https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + architectureFolder + "/" + version + ".dat" + certFlag;
            system(command.c_str());
        }
    }

    if (!onefile && architecture == "x86") {
        // Additional file for Windows 2000
        command = "del " + fullpath + "win2000.dat";
        system(command.c_str());
        command = wget_exe + " -O " + fullpath + "win2000.dat https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + directories[1] + "/win2000.dat" + ((check_cert) ? "" : " --no-check-certificate");
        system(command.c_str());
    }
}