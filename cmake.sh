#!/bin/bash

cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
make -C build/
cd build || exit
ctest
