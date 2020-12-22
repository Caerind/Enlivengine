#!/bin/bash

if [ -d "build" ]; then
	cmake --build build
else
	echo "No 'build' directory"
fi

