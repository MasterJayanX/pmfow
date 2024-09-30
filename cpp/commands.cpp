#define _WIN32_WINNT 0x0500
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <windows.h>
#include <winver.h>
#include <functional>
#include <sstream>
#include <vector>
#include "downloader.cpp"

using namespace std;

class repo {
public:
    repo(const string& os, string operation) {
        // This constructor calls the loadRepos function
        if(operation == "loadRepo"){
            loadRepos(os);
        }
        else if(operation == "loadUninstaller"){
            loadUninstaller();
        }
        else if(operation == "loadUpdater"){
            loadUpdater();
        }
        else if(operation == "loadSilent"){
            loadSilent();
        }
        else{
            cout << "Invalid operation.\n";
        }
    }

    string repos(const string& key) const {
        // This function returns the info of the package
        auto it = packages.find(key);
        return (it != packages.end()) ? it->second : "Package not found";
    }

private:
    // This map contains the packages for each repo
    map<string, string> packages;
    void loadRepos(const string& os) {
        ifstream file;
        string fullpath = programpath + "\\files\\";
        if(os == "Windows 2000"){
            file.open(fullpath + "win2000.dat");
        }
        else if(os == "Windows XP" || os == "Windows XP Professional x64/Windows Server 2003" || os == "ReactOS"){
            file.open(fullpath + "winxp.dat");
        }
        else if(os == "Windows Vista"){
            file.open(fullpath + "winvista.dat");
        }
        else if(os == "Windows 7"){
            file.open(fullpath + "win7.dat");
        }
        else if(os == "Windows 8" || os == "Windows 8.1"){
            file.open(fullpath + "win8.dat");
        }
        else if(os == "Windows 10" || os == "Windows 11"){
            file.open(fullpath + "win10.dat");
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
    void loadUninstaller(){
        // This function loads the uninstaller
        string fullpath = programpath + "\\files\\";
        ifstream file;
        file.open(fullpath + "uninstallers.dat");
        if(!file.is_open()){
            cerr << "Error: uninstallers.dat could not be opened.\n";
            return;
        }
        string line;
        while(getline(file, line)){
            size_t delimiterPos = line.find('=');
            if(delimiterPos != string::npos){
                string key = line.substr(0, delimiterPos);
                string value = line.substr(delimiterPos + 1);
                packages[key] = value;
            }
        }
        file.close();
    }
    void loadUpdater(){
        // This function loads the updater
        string fullpath = programpath + "\\files\\";
        ifstream file;
        file.open(fullpath + "updater.dat");
        if(!file.is_open()){
            cerr << "Error: updater.dat could not be opened.\n";
            return;
        }
        string line;
        while(getline(file, line)){
            size_t delimiterPos = line.find('=');
            if(delimiterPos != string::npos){
                string key = line.substr(0, delimiterPos);
                string value = line.substr(delimiterPos + 1);
                packages[key] = value;
            }
        }
        file.close();
    }
    void loadSilent(){
        // This function loads the silent installers
        string fullpath = programpath + "\\files\\";
        ifstream file;
        file.open(fullpath + "silent-installers.dat");
        if(!file.is_open()){
            cerr << "Error: silent-installers.dat could not be opened.\n";
            return;
        }
        string line;
        while(getline(file, line)){
            size_t delimiterPos = line.find('=');
            if(delimiterPos != string::npos){
                string key = line.substr(0, delimiterPos);
                string value = line.substr(delimiterPos + 1);
                packages[key] = value;
            }
        }
        file.close();
    
    }
};

string getWindowsVersion(){
    // This function returns the Windows version you are running
    string winver;
    if(majorVersion == 4){
        if(build == 1381){
            winver = "Windows NT 4.0 (unsupported)";
        }
        else{
            winver = "Windows 95/98/ME (unsupported)";
        }
    }
    else if(majorVersion == 5 && minorVersion == 0){
        winver = "Windows 2000";
    }
    else if(majorVersion == 5 && (minorVersion == 1)){
        winver = "Windows XP";
    }
    else if(majorVersion == 5 && (minorVersion == 2)){
        if(is_reactos){
            winver = "ReactOS";
        }
        else{
            winver = "Windows XP Professional x64/Windows Server 2003";
        }
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
        if(build <= 19045){
            winver = "Windows 10";
        }
        else if(build >= 22000){
            winver = "Windows 11";
        }
        else{
            winver = "Windows 10";
        }
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

bool updateURL(string url){
    string part;
    size_t startPos = url.find("/download/") + 10;
    size_t endPos = url.find('/', startPos);
    if (startPos != string::npos && endPos != string::npos) {
        part = url.substr(startPos, endPos - startPos);
        istringstream iss(part);
        vector<int> versionNumbers;
        string token;
        while(getline(iss, token, '.')){
            versionNumbers.push_back(stoi(token));
        }
        if (versionNumbers.size() == 3) {
            altmajor = versionNumbers[0];
            altminor = versionNumbers[1];
            altpatch = versionNumbers[2];
            if(altmajor > major || (altminor > minor && altmajor >= major) || (altpatch > patch && altminor  >= minor)){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

void install(char** argv, int argc){
    // This function installs a package
    if(argc < 3){
        cout << "Usage: pmfow install <package>\n";
        log("Usage: pmfow install <package>");
        log("Execution ended with code 1 (error).");
        return;
    }
    else{
        cout << "Installing " << argv[2] << "...\n";
        repo r(winver, "loadRepo");
        repo s(winver, "loadSilent");
        if(r.repos(argv[2]) == "Package not found"){
            cout << "Package not found.\n";
            log("Package not found. The installation of " + string(argv[2]) + " failed.");
            return;
        }
        if(s.repos(argv[2]) == "Package not found"){
            silent = false;
        }
        string url = r.repos(argv[2]);
        string packagename = string(argv[2]);
        cout << url << endl;
        installPackage(packagename, url, s.repos(argv[2]));
        cout << "Done.\n";
        log("Installation of " + packagename + " completed successfully.");
        log("Execution ended with code 0 (success).");
    }
}

void uninstall(char** argv, int argc){
    // This function uninstalls a package
    string command, uninstaller;
    if(argc < 3){
        cout << "Usage: pmfow uninstall <package>\n";
        log("Usage: pmfow uninstall <package>");
        log("Execution ended with code 1 (error).");
        return;
    }
    else{
        cout << "Uninstalling " << argv[2] << "...\n";
        repo r(winver, "loadUninstaller");
        if(r.repos(argv[2]) == "Package not found"){
            cout << argv[2] << "The uninstaller for the specified software could not be found. " << argv[2] << " is either not installed or is installed in a different directory.\n";
            log("The uninstaller for the specified software could not be found. " + string(argv[2]) + " is either not installed or is installed in a different directory.");
            return;
        }
        else{
            uninstaller = r.repos(argv[2]);
            command = uninstaller;
            system(command.c_str());
            cout << "Done.\n";
            log("Uninstallation of " + string(argv[2]) + " completed successfully.");
            log("Execution ended with code 0 (success).");
        }
    }
}

void update(){
    // This function updates the repositories
    if(onlyCheck){
        cout << "Checking for updates...\n";
        Sleep(500);
    }
    else{
        cout << "Updating repositories...\n";
    }
    string url, pmfow;
    repo r(winver, "loadUpdater");
    if(architecture == "x64"){
        url = r.repos("pmfow-updater64");
        if(unstable){
            pmfow = r.repos("pmfow-unstable64");
        }
        else{
            pmfow = r.repos("pmfow64");
        }
    }
    else if(architecture == "x86"){
        url = r.repos("pmfow-updater32");
        if(unstable){
            pmfow = r.repos("pmfow-unstable32");
        }
        else{
            pmfow = r.repos("pmfow32");
        }
    }
    else{
        cout << "Invalid architecture.\n";
        log("Invalid architecture.");
        return;
    }
    log("Updating repositories...");
    bool updateAvailable = updateURL(pmfow);
    if(!onlyCheck){
        updateRepositories(url);
    }
    if(updateAvailable){
        cout << "There is an update available for pmfow (" << altmajor << "." << altminor << "." << altpatch << "). Run \"pmfow-updater\" to install it." << endl;
        log("There is an update available for pmfow (" + to_string(altmajor) + "." + to_string(altminor) + "." + to_string(altpatch) + "). Run \"pmfow-updater\" to install it.");
    }
    else{
        cout << "You are running the latest version of pmfow (" << major << "." << minor << "." << patch << ")." << endl;
        log("You are running the latest version of pmfow (" + to_string(major) + "." + to_string(minor) + "." + to_string(patch) + ").");
    }
}

void list(){
    // This function lists all the packages
    cout << "Listing all packages...\n";
    log("Listing all packages...");
    Sleep(1000);
    ifstream file;
    string fullpath = programpath + "\\files\\";
    if(winver == "Windows 2000"){
        file.open(fullpath + "win2000.dat");
    }
    else if(winver == "Windows XP" || winver == "Windows XP Professional x64/Windows Server 2003"){
        file.open(fullpath + "winxp.dat");
    }
    else if(winver == "Windows Vista"){
        file.open(fullpath + "winvista.dat");
    }
    else if(winver == "Windows 7"){
        file.open(fullpath + "win7.dat");
    }
    else if(winver == "Windows 8" || winver == "Windows 8.1"){
        file.open(fullpath + "win8.dat");
    }
    else if(winver == "Windows 10" || winver == "Windows 11"){
        file.open(fullpath + "win10.dat");
    }
    else{
        cout << "Invalid OS.\n";
        log("Invalid OS.");
        return;
    }
    if (!file.is_open()) {
        cerr << "Repository not found" << endl;
        log("Repository not found.");
        return;
    }
    string line, finalLine;
    int i = 0;
    while (getline(file, line)) {
        finalLine = line;
        size_t delimiterPos = line.find('=');
        if (delimiterPos != string::npos) {
            string key = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);
            cout << key << endl;
            i++;
        }
        Sleep(10);
    }
    string ver = finalLine.substr(finalLine.find(":") + 2);
    int repoversion = stoi(ver);
    cout << "Total packages: " << i << endl;
    cout << "To see a full list of programs and their descriptions, go to https://github.com/MasterJayanX/pmfow/wiki/Software-List." << endl;
    cout << "Version of the repository: " << repoversion << endl;
    log("Total packages: " + to_string(i) + "\n" + "Version of the repository: " + to_string(repoversion));
    file.close();
}

void listUninstall(){
    // This function lists all the packages that can be uninstalled
    cout << "Listing all packages...\n";
    Sleep(1000);
    ifstream file;
    string fullpath = programpath + "\\files\\";
    file.open(fullpath + "uninstallers.dat");
    string line;
    int i = 0;
    while (getline(file, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != string::npos) {
            string key = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);
            cout << key << endl;
            i++;
        }
        Sleep(10);
    }
    cout << "Total packages: " << i << endl;
    log("Total packages that can be uninstalled: " + to_string(i));
    file.close();
}

void search(char** argv, int argc){
    // This function searches for a package
    if(argc < 3){
        cout << "Usage: pmfow search <package>\n";
        log("Usage: pmfow search <package>");
        return;
    }
    else{
        cout << "Searching for " << argv[2] << "...\n";
        repo r(winver, "loadRepo");
        if(r.repos(argv[2]) == "Package not found"){
            cout << "Package not found.\n";
            log("Package not found.");
        }
        else{
            cout << "Package found: " << argv[2] << endl;
            if(show_url){
                cout << "URL: " << r.repos(argv[2]) << endl;
            }
            log("Package found: " + string(argv[2]));
        }
    }
}

void help(){
    // This function shows the help message
    cout << "Usage: pmfow <command> [options]\n";
    cout << "\n";
    cout << "List of commands:\n";
    cout << "pmfow install <package> - Installs a package.\n";
    cout << "pmfow uninstall <package> - Uninstalls a package.\n";
    cout << "pmfow update - Updates the repositories.\n";
    cout << "pmfow search <package> - Searches for a package.\n";
    cout << "pmfow list - Lists all the packages.\n";
    cout << "pmfow help - Shows this help message.\n";
    cout << "pmfow version - Shows the version of pmfow that you are running.\n";
    cout << "pmfow about - Shows information about pmfow.\n";
    cout << "pmfow-updater - Updates the pmfow executable to the latest version.\n";
    cout << "\n";
    cout << "List of flags:\n";
    cout << "pmfow install <package> -f/--force-os <os> - Installs a package for a different OS.\n";
    cout << "pmfow install <package> -c/--check-certificates / pmfow update -c/--check-certificates - Installs a package using wget with certificate checking.\n";
    cout << "pmfow install <package> -w <os>/--wget-version <os> - Installs a package using a different version of wget (options: 2000, xp, win).\n";
    cout << "pmfow update -o/--one-file - Only downloads the repository file that corresponds to the user's Windows version. It can be used alongside --force-os.\n";
    cout << "pmfow update --unstable - Will check if there is a new unstable/development release of pmfow instead of a stable one.\n";
    cout << "pmfow update --check - Checks for pmfow updates when updating the app repositories.\n";
    cout << "pmfow update --only-check - Only checks for pmfow updates and does not update the repositories.\n";
    cout << "pmfow search <package> -f/--force-os <os> - Searches for a package for a different OS.\n";
    cout << "pmfow search <package> -u/--show-url / pmfow search <package> --show-url - Shows the URL of the package.\n";
    cout << "pmfow list --uninstall - Lists all programs that can be uninstalled with pmfow.\n";
    cout << "pmfow version -c/--check - Checks for updates when showing the version of pmfow you are running.\n";
}

void version(){
    cout << programversion << endl;
    log(programversion);
    if(checkUpd){
        string pmfow;
        repo r(winver, "loadUpdater");
        if(architecture == "x64"){
            if(unstable){
                pmfow = r.repos("pmfow-unstable64");
            }
            else{
                pmfow = r.repos("pmfow64");
            }
        }
        else if(architecture == "x86"){
            if(unstable){
                pmfow = r.repos("pmfow-unstable32");
            }
            else{
                pmfow = r.repos("pmfow32");
            }
        }
        else{
            cout << "Invalid architecture.\n";
            return;
        }
        bool updateAvailable = updateURL(pmfow);
        if(updateAvailable){
            cout << "There is an update available for pmfow (" << altmajor << "." << altminor << "." << altpatch << "). Run \"pmfow-updater\" to install it." << endl;
        }
        else{
            cout << "You are running the latest version of pmfow (" << major << "." << minor << "." << patch << ")." << endl;
        }
    }
    log("Execution ended with code 0 (success).");

}
void about(int majorVersion, int minorVersion, int build){
    // This function shows the version of pmfow that you are running
    cout << programversion << endl;
    log(programversion);
    cout << "Made by MasterJayanX" << endl;
    cout << "This program is licensed under the GNU GPL 3 License. See LICENSE for more information." << endl;
    cout << "Github repository: https://github.com/MasterJayanX/pmfow" << endl;
    cout << "Path: " << programpath << endl;
    cout << "Windows Version: " << winver << " (" << majorVersion << "." << minorVersion << "." << build << ")" << endl;
    log("Windows Version: " + winver + " (" + to_string(majorVersion) + "." + to_string(minorVersion) + "." + to_string(build) + ")");
    cout << "Architecture: " << architecture << endl;
    log("Architecture: " + architecture);
    if(majorVersion < 5){
        cout << "Warning: You are using an unsupported version of Windows. You need Windows 2000 or later to use pmfow.\n";
        log("Warning: You are using an unsupported version of Windows. You need Windows 2000 or later to use pmfow.");
    }
    if(checkUpd){
        string pmfow;
        repo r(winver, "loadUpdater");
        if(architecture == "x64"){
            if(unstable){
                pmfow = r.repos("pmfow-unstable64");
            }
            else{
                pmfow = r.repos("pmfow64");
            }
        }
        else if(architecture == "x86"){
            if(unstable){
                pmfow = r.repos("pmfow-unstable32");
            }
            else{
                pmfow = r.repos("pmfow32");
            }
        }
        else{
            cout << "Invalid architecture.\n";
            return;
        }
        bool updateAvailable = updateURL(pmfow);
        if(updateAvailable){
            cout << "There is an update available for pmfow (" << altmajor << "." << altminor << "." << altpatch << "). Run \"pmfow-updater\" to install it." << endl;
            log("There is an update available for pmfow (" + to_string(altmajor) + "." + to_string(altminor) + "." + to_string(altpatch) + "). Run \"pmfow-updater\" to install it.");
        }
        else{
            cout << "You are running the latest version of pmfow (" << major << "." << minor << "." << patch << ")." << endl;
            log("You are running the latest version of pmfow (" + to_string(major) + "." + to_string(minor) + "." + to_string(patch) + ").");
        }
    }
    log("Execution ended with code 0 (success).");
}

int checkFlags(int argc, char** argv){
    // This function checks the flags
    int success = 1;
    if(argc >= 3){
        for(int i = 2; i < argc; i++){
            if(string(argv[i]) == "-c" || string(argv[i]) == "--check-certificates"){
                if(string(argv[1]) != "install" && string(argv[1]) != "update"){
                    cout << "This flag is not compatible with the " << argv[1] << " command.\n";
                    string to_log = "This flag is not compatible with the " + string(argv[1]) + " command.";
                    log(to_log);
                    success = 0;
                    return success;
                }
                else{
                    check_cert = true;
                }
            }
            if(string(argv[i]) == "-f" || string(argv[i]) == "--force-os"){
                if(argc < i+1 || (string(argv[1]) != "install" && string(argv[1]) != "update" && string(argv[1]) != "search" && string(argv[1]) != "list")){
                    string message = "Usage: pmfow install <package> --force-os <os> / pmfow update --force-os <os>";
                    cout << message << endl;
                    cout << "Valid options: 2000, xp, vista, 7, 8, 8.1, 10\n";
                    log(message);
                    success = 0;
                    return success;
                }
                else{
                    if(string(argv[i+1]) == "2000" || string(argv[i+1]) == "Win2000" || string(argv[i+1]) == "win2000"){
                        winver = "Windows 2000";
                    }
                    else if(string(argv[i+1]) == "xp" || string(argv[i+1]) == "XP" || string(argv[i+1]) == "WinXP" || string(argv[i+1]) == "winxp"){
                        winver = "Windows XP";
                    }
                    else if(string(argv[i+1]) == "vista" || string(argv[i+1]) == "Vista" || string(argv[i+1]) == "WinVista" || string(argv[i+1]) == "winvista"){
                        winver = "Windows Vista";
                    }
                    else if(string(argv[i+1]) == "7" || string(argv[i+1]) == "Win7" || string(argv[i+1]) == "win7"){
                        winver = "Windows 7";
                    }
                    else if(string(argv[i+1]) == "8" || string(argv[i+1]) == "Win8" || string(argv[i+1]) == "win8"){
                        winver = "Windows 8";
                    }
                    else if(string(argv[i+1]) == "8.1" || string(argv[i+1]) == "Win8.1" || string(argv[i+1]) == "win8.1"){
                        winver = "Windows 8.1";
                    }
                    else if(string(argv[i+1]) == "10" || string(argv[i+1]) == "Win10" || string(argv[i+1]) == "win10"){
                        winver = "Windows 10";
                    }
                    else{
                        cout << "Invalid OS.\n";
                        success = 0;
                        return success;
                    }
                }
            }
            if(string(argv[i]) == "-u" || string(argv[i]) == "--show-url"){
                show_url = true;
            }
            if(string(argv[i]) == "-o" || string(argv[i]) == "--os-file"){
                if(string(argv[1]) != "update"){
                    cout << "This flag is not compatible with the " << argv[1] << " command.\n";
                    success = 0;
                    return success;
                }
                else{
                    onefile = true;
                }
            }
            if(string(argv[i]) == "-w" || string(argv[i]) == "--wget-version"){
                if(argc < i+1 || (string(argv[1]) != "install" && string(argv[1]) != "update")){
                    cout << "Usage: pmfow install --wget-version <os>\n";
                    cout << "Valid options: 2000, xp, win\n";
                    success = 0;
                    return success;
                }
                else{
                    if(string(argv[i+1]) == "2000" || string(argv[i+1]) == "Win2000" || string(argv[i+1]) == "win2000"){
                        wget_os = 5.0;
                    }
                    else if(string(argv[i+1]) == "xp" || string(argv[i+1]) == "XP" || string(argv[i+1]) == "WinXP" || string(argv[i+1]) == "winxp"){
                        wget_os = 5.1;
                    }
                    else if(string(argv[i+1]) == "standard" || string(argv[i+1]) == "win" || string(argv[i+1]) == "Win"){
                        wget_os = 6.0;
                    }
                    else{
                        cout << "Usage: pmfow install --wget-version <os>\n";
                        cout << "Valid options: 2000, xp, win\n";
                        success = 0;
                        return success;
                    }
                
                }
            }
            if(string(argv[i]) == "--unstable"){
                unstable = true;
            }
            if(string(argv[i]) == "--uninstall"){
                if(string(argv[1]) != "list"){
                    cout << "You are trying to use this flag with a command that isn't list. Usage: pmfow list --uninstall" << endl;
                    success = 0;
                    return success;
                }
                else{
                    list_uninstall = true;
                }
            }
            if(string(argv[i]) == "-ch" || string(argv[i]) == "--check"){
                checkUpd = true;
            }
            if(string(argv[i]) == "--only-check"){
                onlyCheck = true;
            }
        }
    }
    return success;
}