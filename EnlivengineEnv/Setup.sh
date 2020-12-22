#!/bin/bash

if [ -d "build" ]; then
	rm -rf build
fi

./EnlivengineEnv/GenerateProject.sh

./EnlivengineEnv/BuildAll.sh

./EnlivengineEnv/CompileShaders.sh

