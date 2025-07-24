#!/bin/bash


if [ -d "build" ]; then
    rm -r build
    mkdir -p build
else
    mkdir -p build
fi

cd build
cmake .. -DBUILD_SHARED_LIBS=OFF
cmake --build --parallel $(nproc)
