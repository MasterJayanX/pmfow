# Package Manager for Old Windows
 [![GitHub release](https://img.shields.io/github/v/release/MasterJayanX/pmfow.svg)](https://github.com/MasterJayanX/pmfow/releases) [![GitHub all releases](https://img.shields.io/github/downloads/MasterJayanX/pmfow/total)](https://github.com/MasterJayanX/pmfow/releases) 
## Description
pmfow (Package Manager for Old Windows) is a program that allows you to install applications from the command line in older versions of Windows similarly to how you would do it on Linux or on Windows 10 with [winget](https://github.com/microsoft/winget-cli).

## Requirements
pmfow will run in most versions of Windows from Windows XP onwards. It will technically work on Windows 2000, but I flagged that version as "unsupported", so you will need to do some modifications to get it to work.
**This is the list of fully supported versions:**

- Windows XP
- Windows Vista
- Windows 7
- Windows 8
- Windows 8.1

**This is the list of partially supported versions:**

- Windows 10: Windows 10 is partially supported, as most of the packages on Windows 10's repository are the same packages as the ones for Windows 8/8.1. For Windows 10 and 11, use winget instead.
- Windows Server versions from Server 2003 to Server 2012: Only the "regular" versions of Windows are detected, but since the Windows Server releases share the same NT versions as the regular releases, they should work.

## Installation
To install pmfow, you need to follow these steps:
1. Download the .exe file from the Releases page that corresponds to the architecture of your CPU. If you are not sure which one you should pick, go to Control Panel > System and see if it says "32 bit Operating System" or "64 bit Operating System". If it says 32 bit, choose the version that says win32 and if it says 64 bit, choose the version that says win64.
2. Open the .exe file and extract the files in a new folder. To do this, open the .exe, click the three dots, create a new folder and select it.
3. After extracting the files, add the directory to the system's Path. To do this, go to Control Panel > System > Advanced / Advanced Settings > Environment Variables. Then, go to System Variables and find where it says Path. On Windows XP, just double click where it says Path, add a ; at the end of the text window and paste the full directory where you extracted pmfow's files (for example `C:\pmfow`). On newer versions of Windows, click on Path, then click on Edit, click on New and paste the full directory.
## Usage
To use pmfow, open a CMD window and run the following command: `pmfow <command>` (Note: if you didn't follow step 3 of the installation, you will have to open the cmd window in the same folder you extracted the pmfow executable in and type `pmfow.exe <command>` instead.
Here's a list of commands you can use:
- `pmfow install <package>`: This command will allow you to install the program you want.
- `pmfow update`: This will update the repositories, which are just .txt files, but still.
- `pmfow search <package>`: This command will allow you to search for a specific package in the repositories of your operating system.
- `pmfow version`: This command will show you the version of pmfow that you are running, as well as your version of Windows.
- `pmfow help`: This command will show you a list of commands with a description for each one, like the one you are seeing here.

If you are using the install or update commands, you can use these flags:
- `-p` or `--powershell`: Forces the install command to use Powershell's DownloadFile function to download the installation files or update the repository files. Note: This is not needed on Windows 7 or above, since those versions use Powershell by default. For Windows XP and Vista, you must download Powershell 2.0 from [here](https://www.catalog.update.microsoft.com/Search.aspx?q=powershell%202.0) to be able to use it (you will also need to install .NET 2.0 SP1 from [here](https://www.microsoft.com/en-us/download/details.aspx?id=16614)).
- `-w` or `--wget`: Forces the install command to use wget to download the installation files or update the repository files. This is not needed on Windows XP or Vista, since those versions use wget by default. Keep in mind that you will have to install wget on Windows in order to be able to use it with pmfow (I provided links in the step 4 of the Installation).
- `-c` or `--check-certificates`: Forces wget to check for certificates when downloading a package.
- `--force-os`: Forces pmfow to install packages for a different version of Windows.

This is useful if either Powershell or wget fails or just refuses to work.

## Compiling
### Requirements
- Having the g++ compiler installed on your computer (for Windows users, you can install it with MinGW or use WSL).

To compile this program yourself, you need to download the .cpp files and save them in the same directory. Alternatively, you can download the repository files using the following command on a terminal window: `git clone https://github.com/MasterJayanX/pmfow`. 
Then, you have to open a terminal window and compile the main.cpp file with the following command: `g++ main.cpp -o output.exe` (you can replace output with any name you want).

## Limitations
pmfow is in an early stage of development, so you can expect some things to not be working as they should. Some of the limitations it has right now are:
- Unlike other package managers, pmfow will only allow you to install one package at a time.
- The repositories currently have a very limited selection of software.
- If you use the update command in a different directory than the one pmfow is installed in, you will have to copy or move the updated repository files to pmfow's folder manually.
- Uninstalling applications from pmfow and silent installers are not supported yet.

If you find any issues with pmfow, open a report in the Issues section and I will check it out.
