#!/bin/bash

server=$1
port=$2
username=$3
password=$4

project="${5}"
branch="${6}"
config="${7}"
version="${8}"
localPath="${9}"
localConfig="${10}"
distantPath="${11}"

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

extension=""
local_filename=""
if [[ "$platform" == "windows" ]]; then
	extension=".exe"
    local_filename="./build/$localPath/$localConfig/$project$extension"
else
	extension=""
    local_filename="./build/$localPath/$project$extension"
fi

distant_filename="$distantPath$project-$branch-$config-$version-$platform$extension"


echo $local_filename
echo $distant_filename
pwd
ls -al
ls -al build
ls -al build/$localPath
ls -al build/$localPath/$localConfig

curl -u $username:$password -T $local_filename ftp://$server:$port/$distant_filename


