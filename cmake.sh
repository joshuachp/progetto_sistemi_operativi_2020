#!/bin/bash

cmake -Bbuild -GNinja  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
ninja -C build/
