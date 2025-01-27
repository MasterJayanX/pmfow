!include "WinMessages.nsh"

# Define the installer name and output file
OutFile "pmfow_win32_installer.exe"

# Define the default installation directory
InstallDir "$PROGRAMFILES\pmfow"

Name "pmfow"

# Set the directory page
Page directory

# Set the instfiles page
Page instfiles

# Section for the installation
Section "Install pmfow"

  # Set the output path to the installation directory
  SetOutPath "$INSTDIR"
  
  # Copy the program files to the installation directory
  File /r "pmfow32\*.*"

  # Add the installation directory to the system PATH
  Push "$INSTDIR"
  Call AddToPath
  
  # Write the uninstaller executable
  WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

# Function to add the installation directory to the system PATH
Function AddToPath
  Exch $0
  ReadRegStr $1 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path"
  StrCpy $1 "$1;$0"
  WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path" "$1"
  SendMessage ${HWND_BROADCAST} ${WM_SETTINGCHANGE} 0 "STR:Environment"
FunctionEnd

# Required includes
!include "MUI2.nsh"

# Settings for the uninstaller
Section "Uninstall"
  # Remove the program files
  Delete "$INSTDIR\*.*"
  Delete "$INSTDIR\files\*.*"
  RMDir "$INSTDIR\files"
  RMDir "$INSTDIR"

  # Remove the installation directory from the system PATH
  Push "$INSTDIR"
  Call un.RemoveFromPath

SectionEnd

# Function to remove the installation directory from the system PATH
Function un.RemoveFromPath
  Exch $0
  ReadRegStr $1 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path"
  IfErrors done
  StrCpy $2 $1
  StrCpy $3 ""
  loop:
  StrCpy $4 $2 1
  StrCpy $2 $2 "" 1
  ${If} $4 == ";"
    StrCpy $5 $3
    StrCpy $3 ""
    ${If} $5 == $0
      StrCpy $6 ""
    ${Else}
      StrCpy $6 "$6;$5"
    ${EndIf}
  ${Else}
    StrCpy $3 "$3$4"
  ${EndIf}
  ${If} $2 != ""
    Goto loop
  ${EndIf}
  WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path" "$6"
  SendMessage ${HWND_BROADCAST} ${WM_SETTINGCHANGE} 0 "STR:Environment"
  done:
FunctionEnd