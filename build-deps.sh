#!/bin/bash

ROOT_PATH=$(pwd)
cd $ROOT_PATH/3rdparty/pistache
mkdir -p build
cd build
cmake ..
make -j4

