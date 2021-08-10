#!/bin/bash

Help()
{
   echo "Syntax: UberScript [-c][-h|i|g|s|b|t|v|a]"
   echo "options:"
   echo "c     Config"
   echo "h     Help"
   echo "i     Install dependencies"
   echo "g     Generate project"
   echo "s     Shaders compilation"
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
		sudo apt-get install -y cmake gcc-9 g++-9 build-essential mercurial make autoconf automake libtool libasound2-dev libpulse-dev libaudio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libxss-dev libgl1-mesa-dev
	fi
}

GenerateProject()
{
	mkdir -p tmp
	mkdir -p build
	cmake -S . -B build
}

CompileShaders()
{
	path_shaderc=""
	if [[ "$platform" == "windows" ]]; then
		path_shaderc="build/Enlivengine/EnlivengineThirdParty/bgfx/Debug/shaderc.exe"
	else
		path_shaderc="build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc"
	fi
	if [ ! -x "$path_shaderc" ]; then
		cmake --build build --target shaderc --config Debug
	fi

	output_path="build/Shaders"
	path_bgfx_include="Enlivengine/EnlivengineThirdParty/bgfx/bgfx/src"

	mkdir -p "$output_path"
	mkdir -p "$output_path/dx9"
	mkdir -p "$output_path/dx11"
	mkdir -p "$output_path/essl"
	mkdir -p "$output_path/glsl"
	mkdir -p "$output_path/metal"
	mkdir -p "$output_path/pssl"
	mkdir -p "$output_path/spirv"
	
	all_vertex_shaders="`find Enlivengine/Enlivengine/Shaders -name *.vs`"
	for vertex_shader in $all_vertex_shaders
	do
		basename="`basename $vertex_shader`"
		basenamewoext="`echo $basename | cut -f 1 -d '.'`"

		#TODO: platform asm.js ?

		# DX9
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/dx9/${basenamewoext}.vs.bin" --platform windows -p vs_3_0 -O 3 --type vertex --verbose -i $path_bgfx_include

		# DX11
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/dx11/${basenamewoext}.vs.bin" --platform windows -p vs_5_0 -O 3 --type vertex --verbose -i $path_bgfx_include

		# NACL
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/essl/${basenamewoext}.vs.bin" --platform nacl --type vertex --verbose -i $path_bgfx_include

		# Android
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/essl/${basenamewoext}.vs.bin" --platform android --type vertex --verbose -i $path_bgfx_include

		# GLSL
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/glsl/${basenamewoext}.vs.bin" --platform linux -p 120 --type vertex --verbose -i $path_bgfx_include

		# Metal
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/metal/${basenamewoext}.vs.bin" --platform osx -p metal --type vertex --verbose -i $path_bgfx_include

		# PSSL
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/pssl/${basenamewoext}.vs.bin" --platform orbis -p pssl --type vertex --verbose -i $path_bgfx_include

		# Spirv
		${path_shaderc} -f "${vertex_shader}" -o "${output_path}/spirv/${basenamewoext}.vs.bin" --platform linux -p spirv --type vertex --verbose -i $path_bgfx_include
	done

	all_fragment_shaders="`find Enlivengine/Enlivengine/Shaders -name *.fs`"
	for fragment_shader in $all_fragment_shaders
	do
		basename="`basename $fragment_shader`"
		basenamewoext="`echo $basename | cut -f 1 -d '.'`"

		#TODO: platform asm.js ?

		# DX9
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/dx9/${basenamewoext}.fs.bin" --platform windows -p ps_3_0 -O 3 --type fragment --verbose -i $path_bgfx_include

		# DX11
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/dx11/${basenamewoext}.fs.bin" --platform windows -p ps_5_0 -O 3 --type fragment --verbose -i $path_bgfx_include

		# NACL
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/essl/${basenamewoext}.fs.bin" --platform nacl --type fragment --verbose -i $path_bgfx_include

		# Android
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/essl/${basenamewoext}.fs.bin" --platform android --type fragment --verbose -i $path_bgfx_include

		# GLSL
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/glsl/${basenamewoext}.fs.bin" --platform linux -p 120 --type fragment --verbose -i $path_bgfx_include

		# Metal
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/metal/${basenamewoext}.fs.bin" --platform osx -p metal --type fragment --verbose -i $path_bgfx_include

		# PSSL
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/pssl/${basenamewoext}.fs.bin" --platform orbis -p pssl --type fragment --verbose -i $path_bgfx_include

		# Spirv
		${path_shaderc} -f "${fragment_shader}" -o "${output_path}/spirv/${basenamewoext}.fs.bin" --platform linux -p spirv --type fragment --verbose -i $path_bgfx_include
	done
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
	if [[ "$platform" == "windows" ]]; then
		./build/Enlivengine/EnlivengineExamples/$config/PlatformExample.exe # default path for Visual Studio users
		./build/Enlivengine/EnlivengineTests/$config/EnlivengineTests.exe # default path for Visual Studio users
	else
		./build/Enlivengine/EnlivengineExamples/PlatformExample.exe # for Linux/Unix GCC users
		./build/Enlivengine/EnlivengineTests/EnlivengineTests # for Linux/Unix GCC users
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
shaders=false
build=false
tests=false
visual=false

# Options parsing
while getopts c:higsbtva option
do
	case $option in
		c) config=${OPTARG};;
		h) help=true;;
		i) install=true;;
		g) generate=true;;
		s) shaders=true;;
		b) build=true;;
		t) tests=true;;
		v) visual=true;;
		a) 
			install=true
			generate=true
			shaders=true
			build=true
			tests=true
			;;
	esac
done
#echo "=> $config"

if $help; then
	Help
fi
if $install; then
	InstallDependencies
fi
if $generate; then
	GenerateProject
fi
if $shaders; then
	CompileShaders
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