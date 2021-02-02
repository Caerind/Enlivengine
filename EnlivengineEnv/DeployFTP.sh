#!/bin/bash

server="${1}"
port="${2}"
username="${3}"
password="${4}"

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

echo 1:server: "${1}"
echo 2:port: "${2}"
echo 3:username: "${3}"
echo 4:password: "${4}"
echo 5:project: "${5}"
echo 6:branch: "${6}"
echo 7:config: "${7}"
echo 8:version: "${8}"
echo 9:localPath: "${9}"
echo 10:localConfig: "${10}"
echo 11:distantPath: "${11}"
echo $local_filename
echo $distant_filename

curl -u $username:$password -T $local_filename ftp://$server:$port/$distant_filename


