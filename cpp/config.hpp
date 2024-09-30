#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>
#include <winver.h>
#include <time.h>
#define DTTMFMT "%Y-%m-%d %H:%M:%S "
#define DTTMSZ 21

using namespace std;

// Global variables
string winver, architecture, programpath, log_file = "pmfow.log";
bool check_cert, onefile, unstable, silent, list_uninstall, onlyCheck, checkUpd, runasexe, is_reactos, show_url, write_to_log, log_date_time;
float wget_os = 0;
int majorVersion, minorVersion, build;
int major = 0, minor = 3, patch = 1;
int altmajor, altminor, altpatch;
string programversion = "Package Manager for Old Windows v" + to_string(major) + "." + to_string(minor) + "." + to_string(patch) + " (2024-09-30)";
string versionshort = to_string(major) + "." + to_string(minor) + "." + to_string(patch);
bool configExists = true;

class Config {
    // This class reads the config file
public:
    Config(string filename) {
        ifstream file(filename);
        if(!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            configExists = false;
            return;
        }
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
    // This function loads the config file
    string fullpath = programpath + "\\files\\" + "config.ini";
    Config config(fullpath);
    if(!configExists){
        return;
    }
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
    if(config.get("write_to_log") == "true"){
        write_to_log = true;
    }
    else if(config.get("write_to_log") == "false"){
        write_to_log = false;
    }
    if(config.get("log_file") != "pmfow.log"){
        log_file = programpath + "\\" + config.get("log_file");
    }
    else{
        log_file = programpath + "\\" + log_file;
    }
    if(config.get("log_date_time") == "true"){
        log_date_time = true;
    }
    else if(config.get("log_date_time") == "false"){
        log_date_time = false;
    }
}

static char *getDtTm (char *buff) {
    // This function gets the date and time
    time_t t = time (0);
    strftime (buff, DTTMSZ, DTTMFMT, localtime (&t));
    return buff;
}

void log(string message){
    // This function writes to the log file
    ofstream logg;
    if(write_to_log){
        logg.open(log_file, ios::app);
        if(!logg.is_open()){
            return;
        }
        logg << message << endl;
        logg.close();
    }
}

void log_from_main(char** argv, int argc, string message){
    // This function writes to the log file from the main function
    ofstream logg;
    if(write_to_log){
        char buff[DTTMSZ];
        logg.open(log_file, ios::app);
        if(!logg.is_open()){
            return;
        }
        if(log_date_time){
            logg << getDtTm(buff) << endl;
        }
        logg << "pmfow version: " << versionshort << endl;
        logg << "Command: ";
        for(int i = 0; i < argc; i++){
            logg << argv[i] << " ";
        }
        logg << endl;
        if(message != "no_message"){
            logg << message << endl;
        }
        logg << endl;
        logg.close();
    }
}