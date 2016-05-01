#!/bin/bash

mkdir deps-install
cd deps-install

wget https://cmake.org/files/v3.4/cmake-3.4.3-Linux-x86_64.tar.gz
tar xzf cmake-3.4.3-Linux-x86_64.tar.gz
cd cmake-3.4.3-Linux-x86_64
sudo cp -R * /
cd ..

wget https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.zip
unzip -q glfw-3.1.2.zip
cd glfw-3.1.2
cmake -DBUILD_SHARED_LIBS=0 -DGLFW_BUILD_TESTS=0 -DGLFW_BUILD_DOCS=0 -DGLFW_BUILD_EXAMPLES=0 .
make
sudo make install
cd ..

wget https://github.com/g-truc/glm/releases/download/0.9.7.3/glm-0.9.7.3.zip
unzip -q glm-0.9.7.3.zip
cd glm
cmake .
make
sudo make install
cd ..

wget -O assimp-3.2.zip https://github.com/assimp/assimp/archive/v3.2.zip
unzip -q assimp-3.2.zip
cd assimp-3.2.zip
cmake -DASSIMP_BUILD_SAMPLES=false -DASSIMP_BUILD_TESTS=false -DASSIMP_BUILD_ASSIMP_TOOLS=false .
make
sudo make install
cd ..

cd ..