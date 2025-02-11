#!/bin/bash

# Create build directory and build the project
mkdir -p build
cd build
cmake .. && make -j$(nproc)

# Create Release directory in deps/cef if it doesn't exist
mkdir -p ../deps/cef/Release

# Copy CEF resources to the correct location
cp -r ../deps/cef/Resources/* ../deps/cef/Release/