#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>
#include <winver.h>
#include <time.h>
#include <iomanip>

// Defines the date and time format
#define DTTMFMT "%Y-%m-%d %H:%M:%S "
#define DTTMSZ 21
using namespace std;

string getCompileDate() {
    // This function gets the compile date and returns it in a more readable format
    string timestamp = __TIMESTAMP__;
    istringstream iss(timestamp);
    string dayOfWeek, month, day, time, year;
    iss >> dayOfWeek >> month >> day >> time >> year;

    map<string, string> month_map = {
        {"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"},
        {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"},
        {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}
    };

    ostringstream oss;
    oss << year << "-" << month_map[month] << "-" << setw(2) << setfill('0') << day;
    return oss.str();
}

/*
Global variables
----------------
winver: The Windows version
architecture: The architecture of the Windows installation
programpath: The path of the program
log_file: The path of the log file
quiet_cmd: The quiet command for wget
check_cert: Whether to check certificates
onefile: Whether to only update one file
unstable: Whether to check for unstable packages
silent: Whether to use silent installers
list_uninstall: Whether to list uninstallers
onlyCheck: Whether to only check for updates
checkUpd: Whether to check for updates
runasexe: Whether to run compressed files as executables
is_reactos: Whether the OS is ReactOS
show_url: Whether to show the URL of a package
write_to_log: Whether to write to the log file
log_date_time: Whether to log the date and time
upd_config: Whether to update the config file
checkLTS: Whether to check for LTS versions
wget_os: The version of Windows to use for wget
*/
string winver, architecture, programpath, log_file = "pmfow.log", quiet_cmd = " -q --show-progress";
bool check_cert, onefile, unstable, silent, list_uninstall, onlyCheck, checkUpd, runasexe, is_reactos, show_url, write_to_log, log_date_time, upd_config, checkLTS;
float wget_os = 0;

#define COMPILE_DATE ("(" + getCompileDate() + " " + __TIME__ + ")")

// majorVersion, minorVersion, build are used to get the Windows version
int majorVersion, minorVersion, build;
// major, minor, patch are used to get the pmfow version
int major = 0, minor = 4, patch = 3;
int altmajor, altminor, altpatch;
// version is the version of pmfow with the major, minor, and patch variables
string ver = to_string(major) + "." + to_string(minor) + "." + to_string(patch);
// programversion is the full version of pmfow, including the compile date
string programversion = "Package Manager for Old Windows v" + ver + " LTS " + COMPILE_DATE;
bool configExists = true;

class Config {
    // This class reads the config file
public:
    Config(string filename) {
        // This function reads the config file
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
        // This function gets the value of a key
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
        if(config.get("wget_version") == "2k" || config.get("wget_version") == "2000"){
            wget_os = 5.0;
        }
        else if(config.get("wget_version") == "xp" || config.get("wget_version") == "XP"){
            wget_os = 5.1;
        }
        else if(config.get("wget_version") == "win"){
            wget_os = 6.0;
        }
        else{
            wget_os = 6.0;
        }
    }
    // Checks what flags are set in the config file
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
    if(config.get("update_config") == "true"){
        upd_config = true;
    }
    else if(config.get("update_config") == "false"){
        upd_config = false;
    }
    if(config.get("check_lts") == "true"){
        checkLTS = true;
    }
    else if(config.get("check_lts") == "false"){
        checkLTS = false;
    }
}

static char *getDateTime (char *buff) {
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
            logg << getDateTime(buff) << endl;
        }
        logg << "pmfow version: " << ver << endl;
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