#!/bin/bash

if [ -f "CHIP-8" ]; then
    rm CHIP-8
fi
if [ -d "build" ]; then
    rm -r build
    mkdir -p build
else
    mkdir -p build
fi

cd build
cmake .. -DBUILD_SHARED_LIBS=OFF
cmake --build . --parallel $(nproc)
mv CHIP-8 ../
cd ../
rm -r build
