#!/bin/bash

# Platform detection
platform='unknown'
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	platform='linux'
elif [[ "$OSTYPE" == "freebsd"* ]]; then
	platform='bsd'
elif [[ "$OSTYPE" == "darwin"* ]]; then
	platform='mac'
elif [[ "$OSTYPE" == "cygwin" ]]; then
	platform='windows'
elif [[ "$OSTYPE" == "msys" ]]; then
	platform='windows'
elif [[ "$OSTYPE" == "win32" ]]; then
	platform='windows'
else
	echo "OSTYPE == ${OSTYPE} not reconized, assuming it's windows"
	platform='windows'
fi

path_shaderc=""
mkdir -p gfxcompilers
if [[ "$platform" == "windows" ]]; then
	if [ ! -x "build/Enlivengine/EnlivengineThirdParty/bgfx/Debug/shaderc.exe" ]; then
		cmake --build build --target shaderc
	fi
	path_shaderc="gfxcompilers/shaderc.exe"
	cp build/Enlivengine/EnlivengineThirdParty/bgfx/Debug/shaderc.exe $path_shaderc
else
	if [ ! -x "build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc" ]; then
		cmake --build build --target shaderc
	fi
	path_shaderc="gfxcompilers/shaderc"
	cp build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc $path_shaderc
fi

# Ensures shaders are compiled, folders and files are existing, ...
./EnlivengineEnv/CompileShaders.sh

compiledShadersPath="Enlivengine/Enlivengine/Graphics/CompiledShaders/"

# See : https://github.com/bkaradzic/bgfx/blob/master/scripts/shader-embeded.mk