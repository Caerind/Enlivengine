#!/bin/bash

server=""
if [ ! -z "$1" ]; then
	server="$1"
else
    echo "No [1:server] argument, aborting"
	exit 3
fi

port=""
if [ ! -z "$2" ]; then
	port="$2"
else
    echo "No [2:port] argument, aborting"
	exit 3
fi

username=""
if [ ! -z "$3" ]; then
	username="$3"
else
    echo "No [3:username] argument, aborting"
	exit 3
fi

password=""
if [ ! -z "$4" ]; then
	password="$4"
else
    echo "No [4:password] argument, aborting"
	exit 3
fi

distantPath=""
if [ ! -z "$5" ]; then
	distantPath="$5"
else
    echo "No [5:distantPath] argument, aborting"
	exit 3
fi
<<<<<<< HEAD:EnlivengineEnv/DeployGfxCompilers.sh

=======
>>>>>>> 688a00261e3f985955d92eeb869fcc37dd7e61ea:EnlivengineEnv/DeployPrecompiled.sh
config=""
if [ ! -z "$6" ]; then
	config="$6"
else
    echo "No [6:config] argument, aborting"
	exit 3
fi

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

<<<<<<< HEAD:EnlivengineEnv/DeployGfxCompilers.sh
# Path depending on platform/IDE
mkdir -p gfxcompilers
if [[ "$platform" == "windows" ]]; then
	cp "build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/geometryc.exe" "gfxcompilers/geometryc.exe"
	cp "build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/shaderc.exe" "gfxcompilers/shaderc.exe"
	cp "build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/texturec.exe" "gfxcompilers/texturec.exe"
    powershell Compress-Archive gfxcompilers\* gfxcompilers.zip -Force
else
	cp "build/Enlivengine/EnlivengineThirdParty/bgfx/geometryc" "gfxcompilers/geometryc"
	cp "build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc" "gfxcompilers/shaderc"
	cp "build/Enlivengine/EnlivengineThirdParty/bgfx/texturec" "gfxcompilers/texturec"
    zip -r gfxcompilers.zip gfxcompilers
=======
# Precompiled
path_shaderc=""
path_texturec=""
extension=""
if [[ "$platform" == "windows" ]]; then
    path_shaderc="build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/shaderc.exe"
    path_texturec="build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/texturec.exe"
	extension=".exe"
else
    path_shaderc="build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc"
    path_texturec="build/Enlivengine/EnlivengineThirdParty/bgfx/texturec"
fi

if [ ! -x $path_shaderc ]; then
	echo "Shaderc isn't available"
	exit
fi
if [ ! -x $path_texturec ]; then
	echo "Texturec isn't available"
	exit
fi

mkdir -p precompiled
mv $path_shaderc precompiled/shaderc$extension
mv $path_texturec precompiled/texturec$extension

# Path depending on platform/IDE
if [[ "$platform" == "windows" ]]; then
    powershell Compress-Archive precompiled\* precompiled.zip -Force
else
    zip -r precompiled.zip precompiled
>>>>>>> 688a00261e3f985955d92eeb869fcc37dd7e61ea:EnlivengineEnv/DeployPrecompiled.sh
fi

distantFile="${distantPath}gfxcompilers-$config-$platform.zip"

<<<<<<< HEAD:EnlivengineEnv/DeployGfxCompilers.sh
bash EnlivengineEnv/DeploySingleFile.sh $server $port $username $password gfxcompilers.zip $distantFile
=======
bash EnlivengineEnv/DeploySingleFile.sh $server $port $username $password precompiled.zip $distantFile
>>>>>>> 688a00261e3f985955d92eeb869fcc37dd7e61ea:EnlivengineEnv/DeployPrecompiled.sh
