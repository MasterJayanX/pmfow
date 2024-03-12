#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>
#include <winver.h>

using namespace std;

// Global variables
string winver, architecture, programpath;
bool check_cert, onefile, unstable, silent, list_uninstall, onlyCheck, checkUpd, runasexe, is_reactos;
float wget_os = 0;
int majorVersion, minorVersion, build;

class Config {
public:
    Config(string filename) {
        ifstream file(filename);
        string line;
        while(getline(file, line)) {
            if(line[0] != '#' && line[0] != ';' && line[0] != '`'){
                int pos = line.find("=");
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                data[key] = value;
            }
        }
    }
    string get(string key) {
        return data[key];
    }
private:
    map<string, string> data;
};

void loadConfig() {
    string fullpath = programpath + "\\files\\" + "config.ini";
    Config config(fullpath);
    if(config.get("winver") != "auto"){
        winver = config.get("winver");
    }
    if(config.get("wget_version") != "auto"){
        if(config.get("wget_version") == "2k"){
            wget_os = 5.0;
        }
        else if(config.get("wget_version") == "xp"){
            wget_os = 5.1;
        }
        else if(config.get("wget_version") == "win"){
            wget_os = 6.0;
        }
    }
    if(config.get("architecture") != "auto"){
        architecture = config.get("architecture");
    }
    if(config.get("check_for_updates") == "true"){
        checkUpd = true;
    }
    else if(config.get("check_for_updates") == "false"){
        checkUpd = false;
    }
    if(config.get("only_check_for_updates") == "true"){
        onlyCheck = true;
    }
    else if(config.get("only_check_for_updates") == "false"){
        onlyCheck = false;
    }
    if(config.get("only_update_one_file") == "true"){
        onefile = true;
    }
    else if(config.get("only_update_one_file") == "false"){
        onefile = false;
    }
    if(config.get("list_uninstallers") == "true"){
        list_uninstall = true;
    }
    else if(config.get("list_uninstallers") == "false"){
        list_uninstall = false;
    }
    if(config.get("use_silent_installers") == "true"){
        silent = true;
    }
    else if(config.get("use_silent_installers") == "false"){
        silent = false;
    }
    if(config.get("check_for_dev") == "true"){
        unstable = true;
    }
    else if(config.get("check_for_dev") == "false"){
        unstable = false;
    }
    if(config.get("check_certificates") == "true"){
        check_cert = true;
    }
    else if(config.get("check_certificates") == "false"){
        check_cert = false;
    }
    if(config.get("run_compressed_files_as_executables") == "true"){
        runasexe = true;
    }
    else if(config.get("run_compressed_files_as_executables") == "false"){
        runasexe = false;
    }
    if(config.get("is_reactos") == "true"){
        is_reactos = true;
    }
    else if(config.get("is_reactos") == "false"){
        is_reactos = false;
    }
}