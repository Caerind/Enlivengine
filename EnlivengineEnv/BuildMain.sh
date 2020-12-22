#!/bin/bash

if [ -d "build" ]; then
	echo "Skip GenerateProject"
else
	./EnlivengineEnv/GenerateProject.sh
fi

cmake --build build --target Enlivengine
cmake --build build --target EnlivengineTests

