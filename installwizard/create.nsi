!include Sections.nsh
!include env_mani.nsh

!define VERSION 1.0.0
Name "ARMware"

OutFile ".\install.exe"
InstallDir "$PROGRAMFILES\ARMware"

ShowInstDetails show
ShowUninstDetails show
XPStyle on

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Variable
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Var ARMWARE_DIR

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pages
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PageEx Directory
  DirText "Choose the ARMware installation directory" "ARMware Installation Folder"
  DirVar $ARMWARE_DIR
  PageCallbacks wcs_directory_pre
PageExEnd

PageEx instfiles
PageExEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Sections
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

InstType "Full"

Section "ARMware" wcs_section_idx
  SectionIn 1
  WriteRegStr HKLM "Software\ARMware" "Installdir" "$ARMWARE_DIR"
  CreateDirectory $ARMWARE_DIR\bin
  SetOutPath $ARMWARE_DIR\bin
  File "..\release\armware.exe"
  CreateDirectory $ARMWARE_DIR\pixmaps
  SetOutPath $ARMWARE_DIR\pixmaps
  File "..\ARMware\pixmaps\logo.png"
  File "..\ARMware\pixmaps\power_off.png"
  File "..\ARMware\pixmaps\power_on.png"
  File "..\ARMware\pixmaps\reset.png"
  Push "ARMWARE_BASEPATH"
  Push $ARMWARE_DIR
  Call AddToEnvVar
  Push $ARMWARE_DIR\bin
  Call AddToPath
  writeUninstaller "$ARMWARE_DIR\uninstall.exe"
  CreateDirectory $SMPROGRAMS\ARMware
  CreateShortCut "$SMPROGRAMS\ARMware\armware.lnk" "$ARMWARE_DIR\bin\armware.exe"
  CreateShortCut "$SMPROGRAMS\ARMware\uninstall.lnk" "$ARMWARE_DIR\uninstall.exe"
sectionend

section "Uninstall"
  ReadRegStr $ARMWARE_DIR HKLM Software\ARMware "Installdir"
  DeleteRegKey HKLM "Software\ARMware"
  delete "$ARMWARE_DIR\uninstall.exe"
  delete "$SMPROGRAMS\ARMware\uninstall.lnk"
  RMDir /r "$ARMWARE_DIR\bin\"
  RMDir /r "$ARMWARE_DIR\pixmaps\"
  RMDir /r "$ARMWARE_DIR\"
  RMDir /r "$SMPROGRAMS\ARMware\"
  RMDir "$SMPROGRAMS\ARMware\"
  Push "ARMWARE_BASEPATH"
  Push $ARMWARE_DIR
  Call un.RemoveFromEnvVar
  Push $ARMWARE_DIR\bin
  Call un.RemoveFromPath  
SectionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Function wcs_directory_pre
  Push $0
  StrCpy $ARMWARE_DIR "C:\Program Files\ARMware\"
  Pop $0
FunctionEnd
