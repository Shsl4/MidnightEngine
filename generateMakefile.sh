#!/bin/bash

if [ ! command -v cmake &> /dev/null ]
then
    echo "Could not find cmake. Make sure it is installed."
    exit
fi

if [ ! -d "Build" ]
then mkdir Build 
fi

cd Build
cmake .. -G "Unix Makefiles"
