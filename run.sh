#!/bin/bash

# Build the project
cmake -B build .
cmake --build build

# Run the application with required arguments
cd build/bin
./ad-tornado --html ../../assets/index.html --video ../../assets/test.mp4