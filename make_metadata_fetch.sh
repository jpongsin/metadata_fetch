#!/bin/sh
build_folder="build"

if [ -d "$build_folder" ]; then
	rm -rf "$build_folder"
fi

#ensure you are in main folder before building
mkdir "$build_folder"
cd "$build_folder"

cmake --build . --parallel
