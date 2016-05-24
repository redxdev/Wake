#!/bin/bash

mkdir -p deps-install
cd deps-install

ls

if [ ! -d cmake-3.4.3-Linux-x86_64 ]
then
    echo "Downloading cmake..."
    wget https://cmake.org/files/v3.4/cmake-3.4.3-Linux-x86_64.tar.gz
    tar xzf cmake-3.4.3-Linux-x86_64.tar.gz
else
    echo "Using cached cmake download..."
fi
echo "Installing cmake..."
cd cmake-3.4.3-Linux-x86_64
sudo cp -R * /
cd ..

if [ ! -d glfw-3.1.2 ]
then
    echo "Downloading GLFW..."
    wget https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.zip
    unzip -q glfw-3.1.2.zip
    cd glfw-3.1.2
    echo "Building GLFW..."
    cmake -DBUILD_SHARED_LIBS=0 -DGLFW_BUILD_TESTS=0 -DGLFW_BUILD_DOCS=0 -DGLFW_BUILD_EXAMPLES=0 .
    make
else
    echo "Using cached GLFW build..."
    cd glfw-3.1.2
fi
echo "Installing GLFW..."
sudo make install
cd ..

if [ ! -d glm ]
then
    echo "Downloading GLM..."
    wget https://github.com/g-truc/glm/releases/download/0.9.7.3/glm-0.9.7.3.zip
    unzip -q glm-0.9.7.3.zip
    cd glm
    echo "Building GLM..."
    cmake .
    make
else
    echo "Using cached GLM build..."
    cd glm
fi
echo "Installing GLM..."
sudo make install
cd ..

if [ ! -d assimp-3.2 ]
then
    echo "Downloading assimp..."
    wget -O assimp-3.2.zip https://github.com/assimp/assimp/archive/v3.2.zip
    unzip -q assimp-3.2.zip
    cd assimp-3.2
    echo "Building assimp..."
    cmake -DASSIMP_BUILD_SAMPLES=false -DASSIMP_BUILD_TESTS=false -DASSIMP_BUILD_ASSIMP_TOOLS=false .
    make
else
    echo "Using cached assimp build..."
    cd assimp-3.2
fi
echo "Installing assimp..."
sudo make install
cd ..

if [ ! -d snappy-1.1.3 ]
then
    echo "Downloading snappy..."
    wget -O snappy-1.1.3.zip https://github.com/google/snappy/archive/1.1.3.zip
    unzip -q snappy-1.1.3.zip
    cd snappy-1.1.3
    echo "Building snappy..."
    ./autogen.sh
    ./configure
    make
else
    echo "Using cached snappy build..."
    cd snappy-1.1.3
fi
echo "Installing snappy..."
sudo make install
cd ..

cd ..