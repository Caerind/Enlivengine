#!/bin/bash

mkdir -p build
cd build

temp1=""
temp2=""
if [ -z "$1" ]; then
	temp1="-D CMAKE_C_COMPILER=$1"
fi
if [ -z "$2" ]; then
	temp2="-D CMAKE_CXX_COMPILER=$2"
fi
compilerFlags="$temp1 $temp2"

cmake $compilerFlags ..

