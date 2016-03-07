Name "Drawbook"
OutFile "Drawbook_Setup3.exe"

!include "Sections.nsh"
!include "MUI.nsh"

!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_UI "${NSISDIR}\Contrib\UIs\modern.exe"
!define MUI_INSTFILESPAGE_COLORS "FFFFFF 000000"

!insertmacro MUI_PAGE_LICENSE "COPYING.txt"
!insertmacro MUI_PAGE_COMPONENTS

!define MUI_TEXT_LICENSE_TITLE "Agree to the LGPL license"
!define MUI_TEXT_COMPONENTS_TITLE "What do you want to install?"

Page directory
Page instfiles
InstallDir $EXEDIR\Drawbook
DirText "Where will this install to?"

Section "Core"
SetOutPath $INSTDIR
File Drawbook.exe
File Drawbook.ttf
WriteUninstaller $INSTDIR\Uninstall.exe
SectionEnd

Section "Copyright and Docs"
SetOutPath "$EXEDIR\Documentation"
File README.txt
File COPYING.txt
SectionEnd

Section "Library DLLS"
SetOutPath $INSTDIR
File SDL.dll
File libSDL_gfx-15.dll
File SDL_ttf.dll
File libSDL_Button.a
SectionEnd

Section "Source code"
SetOutPath "$EXEDIR\src"
File Makefile
File drawbook.c
File SDL_Button.c
File SDL_Button.h
File libSDL_Button.a
SectionEnd

Section "Library Sources"
SetOutPath "$EXEDIR\Libs"
File SDL_gfx-2.0.25.tar.xz
File SDL_ttf-2.0.11.tar.xz
File SDL-1.2.15.tar.xz
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