# Package Manager for Old Windows
 [![GitHub release](https://img.shields.io/github/v/release/MasterJayanX/pmfow.svg)](https://github.com/MasterJayanX/pmfow/releases) [![GitHub all releases](https://img.shields.io/github/downloads/MasterJayanX/pmfow/total)](https://github.com/MasterJayanX/pmfow/releases) 

## Languages
[English](https://github.com/MasterJayanX/pmfow/blob/main/README.md) | [Español](https://github.com/MasterJayanX/pmfow/blob/main/README_ES.md)
## Description
pmfow (Package Manager for Old Windows) is a program that allows you to install applications from the command line in older versions of Windows similarly to how you would do it in Linux or in Windows 10 with [winget](https://github.com/microsoft/winget-cli).

## Requirements
### Hardware requirements
pmfow will run on anything that runs Windows 2000 or XP and has an internet connection.

**Minimum requirements**
- A CPU
- Some RAM
- Storage (optional)
- An internet connection (not optional)

**Recommended requirements**
- A Pentium or Celeron CPU
- 512 MB of RAM (needed to run versions of Windows newer than XP) or enough RAM to use a web browser
- Enough storage to store at least a couple of .exe files (300 MB of free space should be more than enough)
- A good internet connection (at least 5 Mbps, but even something slightly better than a 56k modem should be enough)

### Software requirements
pmfow will run in most versions of Windows from Windows 2000 onwards.

**This is the list of fully supported versions:**

- Windows XP
- Windows Vista
- Windows 7
- Windows 8
- Windows 8.1

**This is the list of partially supported versions:**

- Windows 10: Windows 10 is partially supported, as most of the packages on Windows 10's repository are the same packages as the ones for Windows 8/8.1. For Windows 10 and 11, use winget instead.
- Windows 2000: Limited selection of packages available for this version of Windows and you can only use it with an old version of wget, which may not work correctly.
- Windows Server versions from Server 2003 to Server 2022: Only the "regular" versions of Windows are detected, but since the Windows Server releases share the same NT versions as the regular releases, they should work (the only exception is Server 2003, but NT 5.2 is supported).

## Installation
To install pmfow, you need to follow these steps:
1. Download the .exe file from the Releases page that corresponds to the architecture of your CPU. If you are not sure which one you should pick, go to Control Panel > System and see if it says "32 bit Operating System" or "64 bit Operating System". If it says 32 bit, choose the version that says win32 and if it says 64 bit, choose the version that says win64.
2. Open the .exe file and extract the files in a new folder. To do this, open the .exe, click the three dots, create a new folder and select it.
3. After extracting the files, add the directory to the system's Path. To do this, go to Control Panel > System > Advanced / Advanced Settings > Environment Variables. Then, go to System Variables and find where it says Path. On Windows XP, just double click where it says Path, add a ; at the end of the text window and paste the full directory where you extracted pmfow's files (for example, `C:\pmfow`). On newer versions of Windows, click on Path, then click on Edit, click on New and paste the full directory.
4. For Windows 2000 and XP users, you will have to change the wget executable that is used by pmfow. To do this, you have two options. 1: Delete the wget.exe file from pmfow's folder, then rename one of the other wget files to wget.exe (for Windows 2000 you have to rename the file named wget_2k.exe and for Windows XP users you have to rename the file named wget_xp.exe). 2: Run the install and update commands with the `--wget-version <version>` flag, changing the `<version>` part with either 2000 or win2000 for Windows 2000 or xp or winxp for Windows XP. Keep in mind that you will have to do this every time, so the first method is recommended.
## Usage
To use pmfow, open a CMD window and run the following command: `pmfow <command>` (Note: if you didn't follow step 3 of the installation, you will have to open the cmd window in the same folder you extracted the pmfow executable in and type `pmfow.exe <command>` instead).
Here's a list of commands you can use:
- `pmfow install <package>`: This command will allow you to install the program you want.
- `pmfow update`: This will update the repositories, which are just .txt files, but still.
- `pmfow search <package>`: This command will allow you to search for a specific package in the repositories of your operating system.
- `pmfow version`: This command will show you the version of pmfow that you are running, as well as your version of Windows.
- `pmfow help`: This command will show you a list of commands with a description for each one, like the one you are seeing here.
- `pmfow list`: This command lists all the available packages in your current repository.

If you are using the install or update commands, you can use these flags:
- `-p` or `--powershell`: Forces the install command to use Powershell's DownloadFile function to download the installation files or update the repository files. For Windows XP and Vista, you must download Powershell 2.0 from [here](https://www.catalog.update.microsoft.com/Search.aspx?q=powershell%202.0) to be able to use it (you will also need to install .NET 2.0 SP1 from [here](https://www.microsoft.com/en-us/download/details.aspx?id=16614)).
- `-w` or `--wget`: Forces the install command to use wget to download the installation files or update the repository files. This is not needed most of the time since wget is used by pmfow by default.
- `-c` or `--check-certificates`: Makes wget check for certificates when downloading a package.
- `--force-os <version>`: Forces pmfow to install packages for a different version of Windows.
- `--show-url`: Shows the URL from which the package you are installing or searching for is downloaded.
- `-o` or `--one-file`: The update command will only update the repository file that corresponds to your current version of Windows.
- `--wget-version <version>`: Forces the install or update command to use a version of wget for a specific version of Windows (for example: Windows XP) instead of renaming the file of the version you need to use.

## Compiling
### Requirements
- Having the g++ compiler installed on your Windows computer (you can install it with MinGW).

To compile this program yourself, you need to download the .cpp files and save them in the same directory. Alternatively, you can download the repository files using the following command on a terminal window: `git clone https://github.com/MasterJayanX/pmfow`. 
Then, you have to open a terminal window and compile the main.cpp file with the following command: `g++ main.cpp -o output.exe` (you can replace output with any name you want).

## Limitations and known issues
pmfow is in an early stage of development, so you can expect some things to not be working as they should. Some of the limitations it has right now are:
- Unlike other package managers, pmfow will only allow you to install one package at a time.
- The repositories currently have a very limited selection of software.
- Uninstalling applications from pmfow and silent installers are not supported yet.
- pmfow doesn't handle spaced directories well, so if you're using Windows XP, don't install pmfow inside the Documents and Settings folder. Instead, you could just create a folder directly in the C: drive and extract the files there.

If you find any issues with pmfow, open an issue in the Issues section and I will check it.

## FAQ (Frequently Asked Questions)
### What versions of Windows are supported?
As I said above, pmfow works on the following Windows versions: Windows 2000, Windows XP, Windows Server 2003, Windows Vista, Windows Server 2008, Windows 7, Windows 8, Windows Server 2012, Windows 8.1, Windows 10, Windows Server 2016/2019/2022 and Windows 11. It should also work on ReactOS, but I haven't tested it yet.
### What are the .txt files that come alongside pmfow's files?
Those .txt files are just the lists of softwares that pmfow can install. They are basically like repositories. I might change them to files without extensions in later updates.
### What applications can I install with pmfow?
You can find the full list of applications that can be installed in [Software_List.md](https://github.com/MasterJayanX/pmfow/blob/main/Software_List.md).
### Does pmfow work on Windows 9x?
Unfortunately, due to these versions of Windows being too old for wget to work reliably on them, I decided to not add support for them with this project.
### Will this project support other operating systems?
Unfortunately no, because other OSes probably already have package managers for them. For example, for old versions of macOS, you can use [Tigerbrew](https://github.com/mistydemeo/tigerbrew) and most Linux distributions come with a package manager.
### What things do you plan to add?
I'm planning to add the option of installing multiple packages at once, more flags to configure the way you install packages or update the repositories, the option of using silent installers and of course, I will add more applications to the repositories.

## Donations
I accept donations through Ko-fi:

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D37FMC3)
