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

# Path depending on platform/IDE
if [[ "$platform" == "windows" ]]; then
    path_shaderc="./build/Enlivengine/EnlivengineThirdParty/bgfx/Debug/shaderc.exe" # default path for Visual Studio users
else
    path_shaderc="./build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc" # for Linux/Unix GCC users
fi

# Check if shaderc exists, if not then try to build
if [ ! -x $path_shaderc ]; then
	if [[ "$platform" == "windows" ]]; then
		MSBuild.exe build/Enlivengine/EnlivengineThirdParty/bgfx/shaderc.vcxproj
	else
		echo "Shaderc haven't been built"
	fi
	
	# Check if shaderc exists a second time, if not exit
	if [ ! -x $path_shaderc ]; then
		echo "Shaderc isn't available"
		exit
	fi
fi

output_path="build/Shaders"

mkdir -p $output_path
mkdir -p "${output_path}/dx9"
mkdir -p "${output_path}/dx11"
mkdir -p "${output_path}/essl"
mkdir -p "${output_path}/glsl"
mkdir -p "${output_path}/metal"
mkdir -p "${output_path}/pssl"
mkdir -p "${output_path}/spirv"

path_bgfx_include="Enlivengine/EnlivengineThirdParty/bgfx/bgfx/src"

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

