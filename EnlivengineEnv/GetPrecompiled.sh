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

distantPath=""
if [ ! -z "$5" ]; then
	distantPath="$5"
else
    echo "No [5:distantPath] argument, aborting"
	exit 3
fi

config=""
if [ ! -z "$6" ]; then
	config="$6"
else
    echo "No [6:config] argument, aborting"
	exit 3
fi

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

distantFile="${distantPath}precompiled-$config-$platform.zip"

bash EnlivengineEnv/GetSingleFile.sh $server $port $username $password precompiled.zip $distantFile 

# Path depending on platform/IDE
if [[ "$platform" == "windows" ]]; then
    powershell Expand-Archive precompiled.zip .
else
    unzip precompiled.zip
fi
