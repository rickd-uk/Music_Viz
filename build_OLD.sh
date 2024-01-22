#!/bin/sh 

set -xe
# Set the project-specific LD_LIBRARY_PATH

# export LD_LIBRARY_PATH=$(pwd)/build:$LD_LIBRARY_PATH

CFLAGS="-Wall -Wextra -ggdb `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw -lm -ldl -lpthread"

mkdir -p ./build
# -fPIC: position independant code (does not depend on absolute addr. in memory)
clang $CFLAGS -o ./build/libplug.so -fPIC -shared ./src/plug.c $LIBS

# clang $CFLAGS -DHOTRELOAD -o ./build/musicviz  ./src/musicviz.c $LIBS -L./build/ 

clang $CFLAGS -DHOTRELOAD -o ./build/musicviz ./src/musicviz.c $LIBS -L./build/ 
# clang $CFLAGS -DHOTRELOAD -o ./build/musicviz ./src/musicviz.c $LIBS -L./build/ -Wl,-rpath,'$(realpath ./build)'
# clang $CFLAGS -o ./build/musicviz ./src/plug.c ./src/musicviz.c $LIBS -L./build/ 
clang -o ./build/fft ./src/fft.c -lm

