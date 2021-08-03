#!/bin/bash

if [ ! -d "build" ]; then
	bash EnlivengineEnv/GenerateProject.sh
fi

project=""
if [ ! -z "$1" ]; then
	project="$1"
else
    echo "No [1:project] argument, aborting"
	exit 3
fi

config=""
if [ ! -z "$2" ]; then
	config="$2"
else
    echo "No [2:config] argument, aborting"
	exit 3
fi

cmake --build build --target ${project} --config ${config}
