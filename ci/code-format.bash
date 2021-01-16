#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
# set -e

# Define colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NORMAL='\033[0m'

apply=0
action="Checking"
if [ $# -eq 1 ]; then
    if [ "$1" == "apply" ]; then
        apply=1
        action="Applying"
    else
        echo "'apply' is the only argument supported"
        exit 1
    fi
fi

files=$(find . -type f -name "*.c" -or -name "*.cpp" -or -name "*.h" -or -name "*.hpp" -or -name "*.ino")

# Check clang-format output
for f in $files ; do
    if [ -f "$f" ]; then
        echo "${action} code format for ${f}"
        if [ $apply -eq 1 ]; then
            clang-format -i $f
        else
            diff $f <(clang-format --assume-filename=main.cpp $f) --minimal
        fi
        if [ $? -ne 0 ]; then
            echo -e "${RED}\xe2\x9c\x96${NORMAL}";  # Cross
            exit 1
        else
            echo -e "${GREEN}\xe2\x9c\x93${NORMAL}";  # Tick
        fi
    fi
done
