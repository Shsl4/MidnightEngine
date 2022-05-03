@echo off
where cmake.exe >nul 2>&1 || echo Could not find cmake.exe. Make sure it is installed. && exit

if not exist "./.build" mkdir "./.build"
attrib +h .build /s /d
cd "./.build"
cmake .. -G "Visual Studio 17 2022"
