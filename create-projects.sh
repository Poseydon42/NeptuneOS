#!/bin/sh

script_dir=$(dirname $0)
cd $script_dir

# System build folder
mkdir -p Build/System
cd Build/System
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../Scripts/CMake/CMakeToolchain.txt -GNinja
cd ../..

# Tests build folder
mkdir -p Build/Tests
cd Build/Tests
cmake ../.. -GNinja
cd ../..

