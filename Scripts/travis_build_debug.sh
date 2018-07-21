#!/usr/bin/env bash

set -e

export NUM_JOBS=1

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
bin/UnitTests

unamestr=`uname`
if [[ "$unamestr" == 'Darwin' ]]; then
    echo "Disabling pip test for macOS"
else
    cd ..
    pip install --user -r requirements.txt
    pip install --user .
    python Tests/PythonTests/main.py
fi