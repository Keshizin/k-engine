#!/bin/bash

./lua ../auto-increment.lua ../../engine/include/k_version.hpp $1 2

sudo mkdir ../temp
cd ../temp

sudo cmake _DCMAKE_BUILD_TYPE=$1 ../..
sudo make -j$(nproc)

cd ..
sudo rm -rf temp
cd linux
