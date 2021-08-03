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

if [[ "$platform" == "linux" ]]; then
	sudo apt-get install -y cmake gcc-9 g++-9 build-essential git make cmake autoconf automake \
libtool pkg-config libasound2-dev libpulse-dev libaudio-dev libjack-dev \
libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \
libxinerama-dev libxxf86vm-dev libxss-dev libgl1-mesa-dev libdbus-1-dev \
libudev-dev libgles2-mesa-dev libegl1-mesa-dev libibus-1.0-dev \
fcitx-libs-dev libsndio-dev libwayland-dev \
libxkbcommon-dev libdrm-dev libgbm-dev
fi
