#!/bin/bash
python3 cugl .
cd build/cmake/cmake
cmake ..
make -j
