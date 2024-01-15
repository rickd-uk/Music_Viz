#!/bin/sh 

set -xe

CFLAGS="-Wall -Wextra `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw -lm -ldl -lpthread"

mkdir -p ./build
clang $CFLAGS -o ./build/musicviz ./src/plug.c ./src/musicviz.c $LIBS
clang -o ./build/fft ./src/fft.c -lm

