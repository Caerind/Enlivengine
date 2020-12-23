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

mkdir -p build
cd build

temp1=""
temp2=""
if [ -z "$1" ]; then
	echo "C Compiler not specified, using default"
else
        echo "C Compiler : $1"
	temp1="-D CMAKE_C_COMPILER=$1"
	if [[ "$platform" == "linux" ]]; then
		export CC="$1"
	fi
fi
if [ -z "$2" ]; then
	echo "C++ Compiler not specified, using default"
else
        echo "C++ Compiler : $2"
	temp2="-D CMAKE_CXX_COMPILER=$2"
	if [[ "$platform" == "linux" ]]; then
		export CC="$2"
	fi
fi
compilerFlags="$temp1 $temp2"
echo "CompilerFlags : $compilerFlags"

cmake ${compilerFlags} ..

