#!/bin/bash

mkdir deps-install
cd deps-install

wget https://cmake.org/files/v3.4/cmake-3.4.3-Linux-x86_64.tar.gz
tar xvzf cmake-3.4.3-Linux-x86_64.tar.gz
cd cmake-3.4.3-Linux-x86_64
sudo cp -R * /
cd ..

wget https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.zip
unzip glfw-3.1.2.zip
cd glfw-3.1.2
cmake -DBUILD_SHARED_LIBS=0 -DGLFW_BUILD_TESTS=0 -DGLFW_BUILD_DOCS=0 -DGLFW_BUILD_EXAMPLES=0 .
make
sudo make install
cd ..

wget https://github.com/g-truc/glm/releases/download/0.9.7.3/glm-0.9.7.3.zip
unzip glm-0.9.7.3.zip
cd glm
cmake .
make
sudo make install
cd ..

cd ..