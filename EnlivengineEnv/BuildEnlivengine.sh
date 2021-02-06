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

./EnlivengineEnv/Build.sh Enlivengine ${config} 
./EnlivengineEnv/Build.sh EnlivengineTest ${config}
./EnlivengineEnv/Build.sh 3DSceneExample ${config}

