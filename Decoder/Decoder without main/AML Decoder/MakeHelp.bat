@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by AML DECODER.HPJ. >"hlp\AML Decoder.hm"
echo. >>"hlp\AML Decoder.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\AML Decoder.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\AML Decoder.hm"
echo. >>"hlp\AML Decoder.hm"
echo // Prompts (IDP_*) >>"hlp\AML Decoder.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\AML Decoder.hm"
echo. >>"hlp\AML Decoder.hm"
echo // Resources (IDR_*) >>"hlp\AML Decoder.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\AML Decoder.hm"
echo. >>"hlp\AML Decoder.hm"
echo // Dialogs (IDD_*) >>"hlp\AML Decoder.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\AML Decoder.hm"
echo. >>"hlp\AML Decoder.hm"
echo // Frame Controls (IDW_*) >>"hlp\AML Decoder.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\AML Decoder.hm"
REM -- Make help for Project AML DECODER


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\AML Decoder.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\AML Decoder.hlp" goto :Error
if not exist "hlp\AML Decoder.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\AML Decoder.hlp" Debug
if exist Debug\nul copy "hlp\AML Decoder.cnt" Debug
if exist Release\nul copy "hlp\AML Decoder.hlp" Release
if exist Release\nul copy "hlp\AML Decoder.cnt" Release
echo.
goto :done

:Error
echo hlp\AML Decoder.hpj(1) : error: Problem encountered creating help file

:done
echo.
