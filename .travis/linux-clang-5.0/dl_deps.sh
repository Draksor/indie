#!/bin/bash

wget -O cmake3.11 https://cmake.org/files/v3.11/cmake-3.11.0-Linux-x86_64.tar.gz
tar xvf cmake3.11
PATH="$PWD/cmake-3.11.0-Linux-x86_64/bin:$PATH"