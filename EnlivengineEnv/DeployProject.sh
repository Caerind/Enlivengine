#!/bin/bash

server=""
if [ ! -z "$1" ]; then
	server="$1"
else
    echo "No [1:server] argument, aborting"
	exit 3
fi

port=""
if [ ! -z "$2" ]; then
	port="$2"
else
    echo "No [2:port] argument, aborting"
	exit 3
fi

username=""
if [ ! -z "$3" ]; then
	username="$3"
else
    echo "No [3:username] argument, aborting"
	exit 3
fi

password=""
if [ ! -z "$4" ]; then
	password="$4"
else
    echo "No [4:password] argument, aborting"
	exit 3
fi


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

# Branch detection
branchBreaker="/"
if [[ "$branch" == *"$branchBreaker"* ]]; then
	branch="${6##*/}"
fi

extension=""
localFile=""
if [[ "$platform" == "windows" ]]; then
	extension=".exe"
    localFile="./build/$localPath$localConfig/$project$extension"
else
	extension=""
    localFile="./build/$localPath$project$extension"
fi

distantFile="$distantPath$project-$branch-$config-$version-$platform$extension"

# Debug
echo $localFile
echo $distantFile
echo server="$server"
echo port="$port"
echo username="$username"
echo password="$password"
echo project="$project"
echo branch="$branch"
echo config="$config"
echo version="$version"
echo localPath="$localPath"
echo localConfig="$localConfig"
echo distantPath="$distantPath"


./EnlivengineEnv/Deploy_SingleFile.sh $server $port $username $password $localFile $distantFile
