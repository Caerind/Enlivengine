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

localFile=""
if [ ! -z "$5" ]; then
	localFile="$5"
else
    echo "No [5:localFile] argument, aborting"
	exit 3
fi

distantFile=""
if [ ! -z "$6" ]; then
	distantFile="$6"
else
    echo "No [6:distantFile] argument, aborting"
	exit 3
fi

curl -u $username:$password ftp://$server:$port/$distantFile -o $localFile


