@echo off
echo Setting environment for using Microsoft Visual Studio 2008 WM6 tools.
set PROGFILES=%ProgramFiles(x86)%
if "%ProgramFiles(x86)%"=="" SET PROGFILES=%ProgramFiles%

set VSINSTALLDIR=%PROGFILES%\Microsoft Visual Studio 9.0
set SDKROOT=%PROGFILES%\Windows Mobile 6 SDK
set OSVERSION=WCE500
set PLATFORM=Windows Mobile 6 Professional SDK
set TARGETCPU=ARMV4I
set UTILDIR=%VSINSTALLDIR%\SmartDevices\SDK\SDKTools
set WindowsSdkDir=c:\Program Files\Microsoft SDKs\Windows\v6.0A\

set VCINSTALLDIR=%VSINSTALLDIR%\VC
set DevEnvDir=%VSINSTALLDIR%\Common7\IDE

set PATH=%VSINSTALLDIR%\VC\ce\bin\x86_arm;%VSINSTALLDIR%\Common7\IDE;%VSINSTALLDIR%\VC\BIN;%VSINSTALLDIR%\Common7\Tools;%VSINSTALLDIR%\Common7\Tools\bin;%VSINSTALLDIR%\SDK\v2.0\bin;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;%VSINSTALLDIR%\VC\VCPackages;%WindowsSdkDir%\bin;%PATH%
set INCLUDE=%PROGFILES%\Windows Mobile 6 SDK\Include\Armv4i;%PROGFILES%\Windows Mobile 6 SDK\PocketPC\Include\Armv4i;%VSINSTALLDIR%\VC\ce\include;%VSINSTALLDIR%\VC\ATLMFC\INCLUDE;%VSINSTALLDIR%\VC\INCLUDE;%VSINSTALLDIR%\SDK\v2.0\include;%INCLUDE%
set LIB=%PROGFILES%\Windows Mobile 6 SDK\PocketPC\Lib\ARMV4I;%VSINSTALLDIR%\VC\ce\lib\armv4i;%VSINSTALLDIR%\VC\ATLMFC\LIB;%VSINSTALLDIR%\VC\LIB;%VSINSTALLDIR%\SDK\v2.0\lib;%LIB%
set LIBPATH=%PROGFILES%\Windows Mobile 6 SDK\PocketPC\Lib\ARMV4I;%VSINSTALLDIR%\VC\ce\lib\armv4i;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;%VSINSTALLDIR%\VC\ATLMFC\LIB
