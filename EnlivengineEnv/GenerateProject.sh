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

# For MinGW, we need to define the -G, elsewhere the default value is fine
generated=false
if [[ "$platform" == "windows" ]]; then
	if [[ "$1" == "gcc" ]]; then
		cmake $compilerFlags -G "MinGW Makefiles" ..
		generated=true
	fi
fi
if [ "$generated" = false ] ; then
	cmake $compilerFlags ..
fi

