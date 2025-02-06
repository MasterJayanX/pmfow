!include "WinMessages.nsh"
!include "MUI2.nsh"
!include "nsDialogs.nsh"

# Define the installer name and output file
OutFile "pmfow_win64_installer.exe"

# Define the default installation directory
InstallDir "C:\pmfow"

Name "pmfow"

# MUI settings
!define MUI_ABORTWARNING
!define MUI_HEADER_TEXT "Welcome to the pmfow Installer"
!define MUI_HEADER_SUBTEXT "This will install pmfow on your computer."
!define MUI_BRANDINGTEXT "pmfow Installer"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
Page custom CustomPageCreate CustomPageLeave
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

# Set version information
VIProductVersion "0.4.1.0"
VIAddVersionKey /LANG=1033 "ProductName" "pmfow"
VIAddVersionKey /LANG=1033 "CompanyName" "MasterJayanX"
VIAddVersionKey /LANG=1033 "FileDescription" "Package Manager for Old Windows"
VIAddVersionKey /LANG=1033 "FileVersion" "1.1"
VIAddVersionKey /LANG=1033 "LegalCopyright" "© MasterJayanX. All rights reserved."

Var Dialog
Var CheckBox

Function CustomPageCreate
  nsDialogs::Create 1018
  Pop $Dialog

  ${If} $Dialog == error
    Abort
  ${EndIf}

  ${NSD_CreateCheckbox} 20u 50u 100% 12u "Add to system PATH (Highly recommended)"
  Pop $CheckBox
  ${NSD_Check} $CheckBox

  nsDialogs::Show
FunctionEnd

Function CustomPageLeave
  ${NSD_GetState} $CheckBox $0
  StrCpy $R0 $0
FunctionEnd

# Section for the installation
Section "Install pmfow"

  # Set the output path to the installation directory
  SetOutPath "$INSTDIR"
  
  # Copy the program files to the installation directory
  File /r "pmfow64\*.*"

  # Add the installation directory to the system PATH if the checkbox is checked
  ${If} $R0 == 1
    Push "$INSTDIR"
    Call AddToPath
  ${EndIf}
  
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

# Settings for the uninstaller
Section "Uninstall"
  # Remove the program files
  Delete "$INSTDIR\*.*"
  Delete "$INSTDIR\files\*.*"
  RMDir "$INSTDIR\files"
  RMDir "$INSTDIR"

  # Remove the installation directory from the system PATH if the checkbox is checked
  ${If} $R0 == 1
    Push "$INSTDIR"
    Call un.RemoveFromPath
  ${EndIf}

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