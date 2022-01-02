#!/bin/bash

if [[ "$1" == "" ]]; 
then 
    echo ERROR: No argument supplied. && exit
fi

VERTEX_FILE_PATH="Shaders/$1/$1.vert"
FRAGMENT_FILE_PATH="Shaders/$1/$1.frag"

if [[ ! -f $VERTEX_FILE_PATH ]]; then echo "ERROR: The file $VERTEX_FILE_PATH does not exist."; exit; fi
if [[ ! -f $FRAGMENT_FILE_PATH ]]; then echo "ERROR: The file $FRAGMENT_FILE_PATH does not exist."; exit; fi

if [[ ! -d "./Build/" ]]; then mkdir "./Build/"; fi
if [[ ! -d "./Build/Resources/" ]]; then mkdir "./Build/Resources"; fi
if [[ ! -d "./Build/Resources/Shaders" ]]; then mkdir "./Build/Resources/Shaders"; fi
if [[ ! -d "./Build/Resources/Shaders/GLSL" ]]; then mkdir "./Build/Resources/Shaders/GLSL"; fi
if [[ ! -d "./Build/Resources/Shaders/Metal" ]]; then mkdir "./Build/Resources/Shaders/Metal"; fi
if [[ ! -d "./Build/Resources/Shaders/SPIRV" ]]; then mkdir "./Build/Resources/Shaders/SPIRV"; fi

VERTEX_FILE="vs_$1.bin"
FRAGMENT_FILE="fs_$1.bin"

./shaderc_macOS -i "./include" -f $VERTEX_FILE_PATH -p 410 -o "./Build/Resources/Shaders/GLSL/$VERTEX_FILE" --platform windows --type vertex
./shaderc_macOS -i "./include" -f $VERTEX_FILE_PATH -p spirv -o "./Build/Resources/Shaders/SPIRV/$VERTEX_FILE" --platform windows --type vertex
./shaderc_macOS -i "./include" -f $VERTEX_FILE_PATH -p metal -o "./Build/Resources/Shaders/Metal/$VERTEX_FILE" --platform osx --type vertex

./shaderc_macOS -i "./include" -f $FRAGMENT_FILE_PATH -p 410 -o "./Build/Resources/Shaders/GLSL/$FRAGMENT_FILE" --platform windows --type fragment
./shaderc_macOS -i "./include" -f $FRAGMENT_FILE_PATH -p spirv -o "./Build/Resources/Shaders/SPIRV/$FRAGMENT_FILE" --platform windows --type fragment
./shaderc_macOS -i "./include" -f $FRAGMENT_FILE_PATH -p metal -o "./Build/Resources/Shaders/Metal/$FRAGMENT_FILE" --platform osx --type fragment

cp -r "./Build/Resources/Shaders" "../../Resources"
