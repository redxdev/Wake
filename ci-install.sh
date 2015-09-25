#!/bin/sh
set -e
if [ ! -d "$HOME/cmake/bin" ]; then
    wget http://cmake.org/files/v3.3/cmake-3.3.2-Linux-x86_64.tar.gz -O /tmp/cmake.tar.gz;
    tar -xvf /tmp/cmake.tar.gz;
    mv cmake-3.3.2-Linux-x86_64 cmake;
else
    echo 'Using cached cmake';
fi

if [ ! -d "$HOME/glm/glm" ]; then
    wget https://github.com/g-truc/glm/releases/download/0.9.7.1/glm-0.9.7.1.zip -O /tmp/glm.zip;
    unzip /tmp/glm.zip -d .;
else
    echo 'Using cached GLM';
fi

if [ ! -d "$HOME/glfw/src" ]; then
    wget https://github.com/glfw/glfw/releases/download/3.1.1/glfw-3.1.1.zip -O /tmp/glfw.zip;
    unzip /tmp/glfw -d .;
    mv glfw-3.1.1 glfw;
    ./cmake/bin/cmake glfw;
    cd glfw
    make;
    cd ..;
else
    echo 'Using cached GLFW';
fi