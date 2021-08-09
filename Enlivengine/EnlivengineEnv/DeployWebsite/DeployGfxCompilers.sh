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

# Path depending on platform/IDE
path_geometryc=""
path_shaderc=""
path_texturec=""
extension=""
if [[ "$platform" == "windows" ]]; then
    path_geometryc="build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/geometryc.exe"
    path_shaderc="build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/shaderc.exe"
    path_texturec="build/Enlivengine/EnlivengineThirdParty/bgfx/${config}/texturec.exe"
	extension=".exe"
else
    path_geometryc="build/Enlivengine/EnlivengineThirdParty/bgfx/geometryc"
    path_shaderc="build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc"
    path_texturec="build/Enlivengine/EnlivengineThirdParty/bgfx/texturec"
	extension=""
fi

if [ ! -x $path_geometryc ]; then
	echo "Geometryc isn't available"
fi
if [ ! -x $path_shaderc ]; then
	echo "Shaderc isn't available"
fi
if [ ! -x $path_texturec ]; then
	echo "Texturec isn't available"
fi

mkdir -p gfxcompilers
cp $path_geometryc gfxcompilers/geometryc$extension
cp $path_shaderc gfxcompilers/shaderc$extension
cp $path_texturec gfxcompilers/texturec$extension

# Path depending on platform/IDE
if [[ "$platform" == "windows" ]]; then
    powershell Compress-Archive gfxcompilers\* gfxcompilers.zip -Force
else
    zip -r gfxcompilers.zip gfxcompilers
fi

distantFile="${distantPath}gfxcompilers-$config-$platform.zip"

bash Enlivengine/EnlivengineEnv/DeployWebsite/DeploySingleFile.sh $server $port $username $password gfxcompilers.zip $distantFile
