#!/bin/bash

if [ -d "build" ]; then
	echo "Skip GenerateProject"
else
	./EnlivengineEnv/GenerateProject.sh
fi

config=""
if [ -z "$1" ]; then
	config="Debug"
else
	config="$1"
fi

cmake --build build --target Enlivengine --config ${config}
cmake --build build --target EnlivengineTests --config ${config}

