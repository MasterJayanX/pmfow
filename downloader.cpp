#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <cstdio>
#include <windows.h>

using namespace std;

string winver, architecture;
bool use_powershell = false, check_cert = false;

void installPackage(string package, string url){
    string path, command, filename;
    path = "C:\\pmfow\\Downloads\\";
    command = "mkdir " + path;
    system(command.c_str());
    if(package != "mypal68" && package != "onecoreapi" && package != "paint.net"){
        filename = package + ".exe";
    }
    else if(package == "libreoffice" || package == "python"){
        filename = package + ".msi";
    }
    else{
        filename = package + ".zip";
    }
    string fullpath = path + filename;
    if(use_powershell){
        command = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('" + url + "', '" + fullpath + "')\"";
    }
    else{
        if(check_cert){
            command = "wget -O " + fullpath + " " + url;
        }
        else{
            command = "wget -O " + fullpath + " " + url + " --no-check-certificate";
        }
    }
    system(command.c_str());
    if(package != "mypal68" && package != "onecoreapi" && package != "paint.net"){
        command = "start " + fullpath;
        system(command.c_str());
    }
    else{
        cout << "Please install " << filename << " manually. You can find the file at C:\\pmfow\\Downloads\\" << endl;
    }
}

void updateRepositories(){
    string command1, command2, command3, command4, command5;
    if(use_powershell){
        if(architecture == "x64"){
            command1 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/winxp.txt')\"";
            command2 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/winvista.txt')\"";
            command3 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win7.txt')\"";
            command4 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win8.txt')\"";
            command5 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win10.txt')\"";
        }
        else if(architecture == "x86"){
            command1 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/winxp.txt')\"";
            command2 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/winvista.txt')\"";
            command3 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win7.txt')\"";
            command4 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win8.txt')\"";
            command5 = "powershell -Command \"(New-Object System.Net.WebClient).DownloadFile('https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win10.txt')\"";
        }
    }
    else{
        if(architecture == "x64"){
            if(check_cert){
                command1 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/winxp.txt";
                command2 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/winvista.txt";
                command3 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win7.txt";
                command4 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win8.txt";
                command5 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win10.txt";
            }
            else{
                command1 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/winxp.txt --no-check-certificate";
                command2 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/winvista.txt --no-check-certificate";
                command3 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win7.txt --no-check-certificate";
                command4 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win8.txt --no-check-certificate";
                command5 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/64%20bit/win10.txt --no-check-certificate";
            }
        }
        else if(architecture == "x86"){
            if(check_cert){
                command1 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/winxp.txt";
                command2 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/winvista.txt";
                command3 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win7.txt";
                command4 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win8.txt";
                command5 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win10.txt";
            }
            else{
                command1 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/winxp.txt --no-check-certificate";
                command2 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/winvista.txt --no-check-certificate";
                command3 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win7.txt --no-check-certificate";
                command4 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win8.txt --no-check-certificate";
                command5 = "wget https://raw.githubusercontent.com/MasterJayanX/pmfow/main/32%20bit/win10.txt --no-check-certificate";
            }
        }
    }
    system(command1.c_str());
    system(command2.c_str());
    system(command3.c_str());
    system(command4.c_str());
    system(command5.c_str());
}