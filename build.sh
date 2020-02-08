#!/bin/bash

BuildType=RELEASE
RED='\033[0;31m'
NC='\033[0m'

if [ -n "$1" ]; then
    if [ "$1" -eq 1 ]; then
        BuildType=DEBUG
    fi
fi

echo -e "CMAKE_BUILD_TYPE is set to: ${RED}$BuildType${NC}"
echo "building tools..."

cd tools/CameraOpenNI
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE
make -j

echo "building SLAM..."

cd ../../../
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=$BuildType
make -j
