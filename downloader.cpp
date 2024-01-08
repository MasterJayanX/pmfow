#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>

using namespace std;

string winver, architecture;
bool use_powershell;

void installPackage(string package, string url){
    string path, command;
    path = "C:\\pmfow\\Downloads\\";
    string filename = package + ".exe";
    string fullpath = path + filename;
    if(use_powershell){
        command = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('" + url + "', '" + fullpath + "')\"";
    }
    else{
        command = "wget -O " + fullpath + filename + " " + url;
    }
    system(command.c_str());
    command = "start " + fullpath;
    system(command.c_str());
}

void updateRepositories(){
    string command;
    if(use_powershell){
        command = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/PMF-Official/pmfow/main/pmfow.exe', 'C:\\pmfow\\pmfow.exe')\"";
    }
    else{
        command = "wget -O C:\\pmfow\\pmfow.exe https://raw.githubusercontent.com/PMF-Official/pmfow/main/pmfow.exe";
    }
    system(command.c_str());
}