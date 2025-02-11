#!/bin/bash

# Create deps directory if it doesn't exist
mkdir -p deps

# Download and setup CEF if not present
if [ ! -d "deps/cef" ]; then
    echo "Downloading CEF..."
    wget -O cef.tar.bz2 "https://cef-builds.spotifycdn.com/cef_binary_132.3.2%2Bg4997b2f%2Bchromium-132.0.6834.161_linux64.tar.bz2"
    
    echo "Extracting CEF..."
    tar xf cef.tar.bz2
    mv cef_binary_*_linux64 deps/cef
    
    echo "Cleaning up CEF archive..."
    rm cef.tar.bz2
    
    echo "CEF has been downloaded and extracted. Please build it manually following the instructions in README.md"
fi

# Download and setup OpenFrameworks if not present
if [ ! -d "deps/openFrameworks" ]; then
    echo "Downloading OpenFrameworks..."
    wget -O of.tar.gz "https://github.com/openframeworks/openFrameworks/releases/download/0.12.0/of_v0.12.0_linux64gcc6_release.tar.gz"
    
    echo "Extracting OpenFrameworks..."
    tar xf of.tar.gz
    mv of_v0.12.0_linux64gcc6_release deps/openFrameworks
    
    echo "Cleaning up OpenFrameworks archive..."
    rm of.tar.gz
    
    echo "OpenFrameworks has been downloaded and extracted. Please follow the build instructions in README.md"
fi

echo "Dependencies setup completed!" 