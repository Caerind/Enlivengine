#!/bin/bash

Help()
{
   echo "Syntax: UberScript [-c][-h|i|g|b|t|v|a]"
   echo "options:"
   echo "c     Config"
   echo "h     Help"
   echo "i     Install dependencies"
   echo "g     Generate project"
   echo "b     Build engine"
   echo "t     Tests"
   echo "v     VisualStudio"
   echo "a     All-in-one"
   echo
}

InstallDependencies()
{
    git submodule update --init --recursive

    if [[ "$platform" == "linux" ]]; then
        sudo apt-get install -y cmake gcc-9 g++-9 build-essential mercurial make autoconf automake 
        sudo apt install \
            libxrandr-dev \
            libxcursor-dev \
            libudev-dev \
            libfreetype-dev \
            libopenal-dev \
            libflac-dev \
            libvorbis-dev \
            libgl1-mesa-dev \
            libegl1-mesa-dev 
    fi
}

GenerateProject()
{
	mkdir -p tmp
	mkdir -p build
	cmake -S . -B build
}

BuildEnlivengine()
{
	cmake --build build --target Enlivengine --config $config
	cmake --build build --target EnlivengineTests --config $config
	cmake --build build --target PlatformExample --config $config
	cmake --build build --target 3DSceneExample --config $config
}

Tests()
{
	# Ensures they are both up to date
	cmake --build build --target EnlivengineTests --config $config
	cmake --build build --target PlatformExample --config $config
	
	if [[ "$platform" == "windows" ]]; then
		./build/examples/$config/PlatformExample.exe # default path for Visual Studio users
		./build/tests/$config/EnlivengineTests.exe # default path for Visual Studio users
	else
		./build/examples/PlatformExample.exe # for Linux/Unix GCC users
		./build/tests/EnlivengineTests # for Linux/Unix GCC users
	fi
}

VisualStudio()
{
	if [[ "$platform" == "windows" ]]; then
		all_solutions="`find build -maxdepth 1 -name *.sln`"
		for solution in $all_solutions
		do
			start $solution
		done
	fi
}

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

# Options
config="Debug"
help=false
install=false
generate=false
build=false
tests=false
visual=false

default=true

# Options parsing
while getopts c:higbtva option
do
	case $option in
		c) 
			default=false
			config=$OPTARG
			;;
		h) 
			default=false
			help=true
			;;
		i) 
			default=false
			install=true
			;;
		g) 
			default=false
			generate=true
			;;
		b) 
			default=false
			build=true
			;;
		t) 
			default=false
			tests=true
			;;
		v) 
			default=false
			visual=true
			;;
		a) 
			default=false
			install=true
			generate=true
			build=true
			tests=true
			;;
	esac
done
#echo "=> $config"

# If no arguments, consider it as -a
if $default; then
	install=true
	generate=true
	build=true
	tests=true
fi

# Handle options
if $help; then
	Help
fi
if $install; then
	InstallDependencies
fi
if $generate; then
	GenerateProject
fi
if $build; then
	BuildEnlivengine
fi
if $tests; then
	Tests
fi
if $visual; then
	VisualStudio
fi

echo 