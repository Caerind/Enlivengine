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

localFile=""
if [ ! -z "$3" ]; then
	localFile="$3"
else
    echo "No [3:localFile] argument, aborting"
	exit 3
fi

distantFile=""
if [ ! -z "$4" ]; then
	distantFile="$4"
else
    echo "No [4:distantFile] argument, aborting"
	exit 3
fi

curl http://$server:$port/$distantFile -o $localFile


