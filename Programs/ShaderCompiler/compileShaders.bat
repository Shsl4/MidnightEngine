@echo off

if "%1" == "" echo ERROR: No argument supplied. && exit

set vertexFilePath=Shaders/%1/%1.vert
set fragmentFilePath=Shaders/%1/%1.frag

if not exist %vertexFilePath% echo ERROR: The file (%vertexFilePath%) does not exist. && exit
if not exist %fragmentFilePath% echo ERROR: The file (%fragmentFilePath%) does not exist. && exit

if not exist ".\Build\" mkdir ".\Build"
if not exist ".\Build\Resources" mkdir ".\Build\Resources"
if not exist ".\Build\Resources\Shaders" mkdir ".\Build\Resources\Shaders"
if not exist ".\Build\Resources\Shaders\DX9" mkdir ".\Build\Resources\Shaders\DX9"
if not exist ".\Build\Resources\Shaders\DX11" mkdir ".\Build\Resources\Shaders\DX11"
if not exist ".\Build\Resources\Shaders\GLSL" mkdir ".\Build\Resources\Shaders\GLSL"
if not exist ".\Build\Resources\Shaders\Metal" mkdir ".\Build\Resources\Shaders\Metal"
if not exist ".\Build\Resources\Shaders\SPIRV" mkdir ".\Build\Resources\Shaders\SPIRV"

set vertexFile=vs_%~n1.bin
set fragmentFile=fs_%~n1.bin

.\shaderc.exe -i "./include" -f %vertexFilePath% -p vs_5_0 -o ".\Build\Resources\Shaders\DX11\%vertexFile%" --platform windows --type vertex -O 3
.\shaderc.exe -i "./include" -f %vertexFilePath% -p vs_3_0 -o ".\Build\Resources\Shaders\DX9\%vertexFile%" --platform windows --type vertex -O 3
.\shaderc.exe -i "./include" -f %vertexFilePath% -p 410 -o ".\Build\Resources\Shaders\GLSL\%vertexFile%" --platform windows --type vertex
.\shaderc.exe -i "./include" -f %vertexFilePath% -p spirv -o ".\Build\Resources\Shaders\SPIRV\%vertexFile%" --platform windows --type vertex
.\shaderc.exe -i "./include" -f %vertexFilePath% -p metal -o ".\Build\Resources\Shaders\Metal\%vertexFile%" --platform osx --type vertex

.\shaderc.exe -i "./include" -f %fragmentFilePath% -p ps_5_0 -o ".\Build\Resources\Shaders\DX11\%fragmentFile%" --platform windows --type fragment -O 3
.\shaderc.exe -i "./include" -f %fragmentFilePath% -p ps_3_0 -o ".\Build\Resources\Shaders\DX9\%fragmentFile%" --platform windows --type fragment -O 3
.\shaderc.exe -i "./include" -f %fragmentFilePath% -p 410 -o ".\Build\Resources\Shaders\GLSL\%fragmentFile%" --platform windows --type fragment
.\shaderc.exe -i "./include" -f %fragmentFilePath% -p spirv -o ".\Build\Resources\Shaders\SPIRV\%fragmentFile%" --platform windows --type fragment
.\shaderc.exe -i "./include" -f %fragmentFilePath% -p metal -o ".\Build\Resources\Shaders\Metal\%fragmentFile%" --platform osx --type fragment

if errorlevel 0 (
	xcopy /E /I /Y .\Build\Resources ..\..\Resources
)


