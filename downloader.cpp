#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>
#include <vector>

using namespace std;

string winver, architecture, programpath;
bool use_powershell = false, check_cert = false, show_url = false, onefile = false;
float wget_os = 0;

void installPackage(string package, string url){
    // Install package
    string command, filename, wget_exe = "wget", fullpath = programpath + "\\";
    system(command.c_str());
    if(package != "mypal68" && package != "xpchrome" && package != "onecoreapi" && package != "paint.net" && package != "blendercompat" && package != "libreoffice" && package != "python" && package != "clamav" 
    && package != "multimc" && (package != "blender" || winver == "Windows 2000") && (package == "python3" && (winver != "Windows 2000" && winver != "Windows XP" && winver != "Windows XP Professional x64/Windows Server 2003" && winver != "Windows Vista"))
    && package != "imdisk"){
        filename = package + ".exe";
    }
    else if(package == "libreoffice" || package == "python" || package == "clamav" || (package == "blender" && winver != "Windows 2000") || (package == "python3" && (winver == "Windows 2000" || winver == "Windows XP" 
    || winver == "Windows XP Professional x64/Windows Server 2003" || winver == "Windows Vista"))){
        filename = package + ".msi";
    }
    else{
        filename = package + ".zip";
    }
    fullpath += filename;
    if(show_url){
        cout << "URL: " << url << endl;
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
    if(use_powershell){
        command = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('" + url + "', '" + fullpath + "')\"";
    }
    else{
        if(check_cert){
            command = wget_exe + " -O " + fullpath + " " + url;
        }
        else{
            command = wget_exe + " -O " + fullpath + " " + url + " --no-check-certificate";
        }
    }
    system(command.c_str());
    if (package != "mypal68" && package != "onecoreapi" && package != "paint.net" && package != "xpchrome" && package != "blendercompat" && package != "multimc" && package != "imdisk") {
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
        cout << "Please install " << filename << " manually. You can find the file at pmfow's root folder." << endl;
    }
}

vector<string> repoDirectories(){
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
        cout << "Error: directories.txt not found. The default directories will be used." << endl;
        directories[0] = "64%20bit";
        directories[1] = "32%20bit";
    }
    return directories;
}

void updateRepositories() {
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
    else if(winver == "Windows 10"){
        file_winver = "win10";
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
    if (use_powershell) {
        string architectureFolder = (architecture == "x64") ? directories[0] : directories[1];
        string versionFile = (architectureFolder + "/" + file_winver + ".txt");
        if (onefile) {
            command = "del " + fullpath + file_winver + ".txt";
            system(command.c_str());
            command = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + versionFile + "', '" + fullpath + file_winver + ".txt" + "')\"";
            system(command.c_str());
        } else {
            command = "del " + fullpath + "directories.txt";
            system(command.c_str());
            command = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/directories.txt', '" + fullpath + "directories.txt" + "')\"";
            system(command.c_str());
            for (const auto& version : {"winxp", "winvista", "win7", "win8", "win10"}) {
                command = "del " + fullpath + version + ".txt";
                system(command.c_str());
                command = "del " + fullpath + "directories.txt";
                system(command.c_str());
                command = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + architectureFolder + "/" + version + ".txt', '" + fullpath + version + ".txt" + "')\"";
                system(command.c_str());
            }
        }
    } else {
        string certFlag = (check_cert) ? "" : " --no-check-certificate";
        string architectureFolder = (architecture == "x64") ? directories[0] : directories[1];
        string versionFile = (architectureFolder + "/" + file_winver + ".txt");

        if (onefile) {
            command = "del " + fullpath + file_winver + ".txt";
            system(command.c_str());
            command = wget_exe + " -O " + fullpath + file_winver + ".txt" + " https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + versionFile + certFlag;
            system(command.c_str());
        } else {
            command = "del " + fullpath + "directories.txt";
            system(command.c_str());
            command = wget_exe + " -O " + fullpath + "directories.txt https://raw.githubusercontent.com/MasterJayanX/pmfow/main/directories.txt" + certFlag;
            system(command.c_str());
            for (const auto& version : {"winxp", "winvista", "win7", "win8", "win10"}) {
                command = "del " + fullpath + version + ".txt";
                system(command.c_str());
                command = wget_exe + " -O " + fullpath + version + ".txt" + " https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + architectureFolder + "/" + version + ".txt" + certFlag;
                system(command.c_str());
            }
        }
    }

    if (!onefile && architecture == "x86") {
        // Additional file for Windows 2000
        command = "del " + fullpath + "win2000.txt";
        system(command.c_str());
        command = wget_exe + " -O " + fullpath + "win2000.txt https://raw.githubusercontent.com/MasterJayanX/pmfow/main/" + directories[1] + "/win2000.txt" + ((check_cert) ? "" : " --no-check-certificate");
        system(command.c_str());
    }
}