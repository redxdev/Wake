#!/bin/sh
set -e
if [ ! -d "$HOME/cmake/bin" ]; then
    wget http://cmake.org/files/v3.3/cmake-3.3.2-Linux-x86_64.tar.gz -O /tmp/cmake.tar.gz;
    tar -xvf /tmp/cmake.tar.gz;
    mv cmake-3.3.2-Linux-x86_64 cmake;
else
    echo 'Using cached cmake';
fi

if [ ! -d "$HOME/glm" ]; then
    wget https://github.com/g-truc/glm/releases/download/0.9.7.1/glm-0.9.7.1.zip -O /tmp/glm.zip;
    unzip /tmp/glm.zip;
else
    echo 'Using cached GLM';
fi