@echo off
where cmake.exe >nul 2>&1 || echo Could not find cmake.exe. Make sure it is installed. && exit

if not exist "./Build" mkdir "./Build"
cd "./Build"
cmake .. -G "Visual Studio 17 2022"
#start MidnightEngine.sln