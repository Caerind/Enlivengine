#!/bin/bash

if [ -d "build" ]; then
	rm -rf build
fi

./EnlivengineEnv/GenerateProject.sh

# ShaderC
./EnlivengineEnv/CompileShaders.sh

# Enlivengine
MSBuild.exe build/Enlivengine/Enlivengine/Enlivengine.vcxproj

./EnlivengineEnv/StartVisual.sh

