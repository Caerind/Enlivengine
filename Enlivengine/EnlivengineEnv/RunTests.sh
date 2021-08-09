#!/bin/bash

# Platform detection
platform='unknown'
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	platform='linux'
elif [[ "$OSTYPE" == "freebsd"* ]]; then
	platform='bsd'
elif [[ "$OSTYPE" == "darwin"* ]]; then
	platform='mac'
elif [[ "$OSTYPE" == "cygwin" ]]; then
	platform='windows'
elif [[ "$OSTYPE" == "msys" ]]; then
	platform='windows'
elif [[ "$OSTYPE" == "win32" ]]; then
	platform='windows'
else
	echo "OSTYPE == ${OSTYPE} not reconized, assuming it's windows"
	platform='windows'
fi

if [[ "$platform" == "windows" ]]; then
	config=""
	if [ ! -z "$1" ]; then
		config="$1"
	else
		config="Debug"
	fi
    ./build/Enlivengine/EnlivengineExamples/${config}/PlatformExample.exe # default path for Visual Studio users
    ./build/Enlivengine/EnlivengineTests/${config}/EnlivengineTests.exe # default path for Visual Studio users
else
    ./build/Enlivengine/EnlivengineExamples/PlatformExample # for Linux/Unix GCC users
    ./build/Enlivengine/EnlivengineTests/EnlivengineTests # for Linux/Unix GCC users
fi

