#!/bin/bash
set -ev
sudo apt-get purge cmake
mkdir cmake
cd cmake
wget https://cmake.org/files/v3.9/cmake-3.9.4.tar.gz
tar -xzvf cmake-3.9.4.tar.gz > /dev/null
cd cmake-3.9.4/
./bootstrap > /dev/null
make -j2 > /dev/null
sudo make install > /dev/null
cd ../..
