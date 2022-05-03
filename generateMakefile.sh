#!/bin/bash

if [ ! command -v cmake &> /dev/null ]
then
    echo "Could not find cmake. Make sure it is installed."
    exit
fi

if [ ! -d ".build" ]
then mkdir .build 
fi

cd .build
cmake .. -G "Unix Makefiles"
