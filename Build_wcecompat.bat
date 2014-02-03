@Rem Prerequisites: WM61_ARMV4I_env.bat, Visual Studio 2008
if "%OSVERSION%"=="" call WM6_ARMV4I_env.bat
nmake
