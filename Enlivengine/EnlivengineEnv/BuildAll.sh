#!/bin/bash

if [ ! -d "build" ]; then
	bash Enlivengine/EnlivengineEnv/GenerateProject.sh
fi

config=""
if [ ! -z "$1" ]; then
	config="$1"
else
    echo "No [1:config] argument, aborting"
	exit 3
fi

cmake --build build --config ${config}

