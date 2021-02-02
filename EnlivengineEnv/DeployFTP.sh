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


echo 1 "${1}"
echo 2 "${2}"
echo 3 "${3}"
echo 4 "${4}"
echo 5 "${5}"
echo 6 "${6}"
echo 7 "${7}"
echo 8 "${8}"
echo 9 "${9}"
echo 10 "${10}"
echo 11 "${11}"

echo 1 $server
echo 2 $port
echo 3 $username
echo 4 $password
echo 5 $project
echo 6 $branch
echo 7 $config
echo 8 $version
echo 9 $localPath
echo 10 $localConfig
echo 11 $distantPath


echo $local_filename
echo $distant_filename
echo pwd
pwd
echo ls -al
ls -al
echo ls -al build
ls -al build
echo ls -al build/$localPath
ls -al build/$localPath
echo ls -al build/$localPath/$localConfig
ls -al build/$localPath/$localConfig

curl -u $username:$password -T $local_filename ftp://$server:$port/$distant_filename


