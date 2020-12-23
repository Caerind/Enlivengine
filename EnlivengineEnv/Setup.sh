#!/bin/bash

if [ -d "build" ]; then
	rm -rf build
fi

./EnlivengineEnv/GenerateProject.sh

./EnlivengineEnv/BuildMain.sh

./EnlivengineEnv/CompileShaders.sh

