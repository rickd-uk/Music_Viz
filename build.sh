#!/usr/bin/env bash


set -xe
# Print the current working directory
echo "Current Directory: $(pwd)"

# List the files in the current directory
ls -l

source .env


CFLAGS="-Wall -Wextra -ggdb `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw -lm -ldl -lpthread"

mkdir -p ./build
# -fPIC: position independent code (does not depend on absolute addr. in memory)
clang $CFLAGS -o ./build/libplug.so -fPIC -shared ./src/plug.c $LIBS

clang $CFLAGS -DHOTRELOAD -o ./build/musicviz ./src/musicviz.c $LIBS -L./build/ 
clang -o ./build/fft ./src/fft.c -lm

# Run the program with the specified arguments
# ./build/musicviz "$@"
set +x
