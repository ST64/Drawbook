Name "Drawbook"
OutFile "Drawbook_Setup_V0.6.1.exe"

!include "Sections.nsh"
!include "MUI.nsh"

!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_UI "${NSISDIR}\Contrib\UIs\modern.exe"
!define MUI_INSTFILESPAGE_COLORS "FFFFFF 000000"

!insertmacro MUI_PAGE_LICENSE "COPYING.txt"
!insertmacro MUI_PAGE_COMPONENTS

LangString MUI_TEXT_LICENSE_TITLE ${LANG_ENGLISH} "Agree to the LGPL license"
LangString MUI_TEXT_COMPONENTS_TITLE ${LANG_ENGLISH} "What do you want to install?"

Page directory
Page instfiles
InstallDir $EXEDIR\Drawbook
DirText "Where will this install to?"

Section "Core"
SetOutPath $INSTDIR
File Drawbook.exe
SetOutPath $INSTDIR\\Data
File Data\\Drawbook.ttf
WriteUninstaller $INSTDIR\Uninstall.exe
SectionEnd

Section "DLLs"
SetOutPath $INSTDIR
File libgcc_s_seh-1.dll
File libstdc++-6.dll
File sfml-graphics-2.dll
File sfml-system-2.dll
File sfml-window-2.dll
SectionEnd

Section "Copyright and Docs"
SetOutPath "$EXEDIR\Documentation"
File README.md
File COPYING.txt
SectionEnd

Section "Source code"
SetOutPath "$EXEDIR\src"
File src.tar.gz
SectionEnd

Section "Uninstall"
Delete $INSTDIR\Uninstall.exe
Delete $INSTDIR\Drawbook.exe
RMDir $INSTDIR
Delete "$SMPROGRAMS\Drawbook\Run Drawbook.lnk"
RMDir "$SMPROGRAMS\Drawbook"
SectionEnd

;Section "Start Menu"
;CreateDirectory "$SMPROGRAMS\Drawbook"
;CreateShortCut "$SMPROGRAMS\Drawbook\Run Drawbook.lnk" "$INSTDIR\Drawbook.exe"
;SectionEnd
