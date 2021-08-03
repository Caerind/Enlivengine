#!/bin/bash

if [ ! -d "build" ]; then
	./EnlivengineEnv/GenerateProject.sh
fi

config=""
if [ ! -z "$1" ]; then
	config="$1"
else
    echo "No [1:config] argument, aborting"
	exit 3
fi

cmake --build build --target Enlivengine --config ${config}
cmake --build build --target EnlivengineTests --config ${config}
cmake --build build --target PlatformExample --config ${config}
cmake --build build --target 3DSceneExample --config ${config}

