#!/bin/sh

set -xe
# Set the project-specific LD_LIBRARY_PATH

# Set the project-specific LD_LIBRARY_PATH

# Get the absolute path of the current directory
current_dir=$(readlink -f "$(pwd)")

# Disable debugging for the next command
set +x

# Check if the directory is already in LD_LIBRARY_PATH
if [[ ":$LD_LIBRARY_PATH:" != *":$current_dir/build:"* ]]; then
    # If not present, add it to LD_LIBRARY_PATH
    export LD_LIBRARY_PATH="$current_dir/build:$LD_LIBRARY_PATH"
else
    echo "Directory $current_dir/build is already in LD_LIBRARY_PATH."
fi

# Re-enable debugging for the rest of the script
set -x


CFLAGS="-Wall -Wextra -ggdb `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw -lm -ldl -lpthread"

mkdir -p ./build
# -fPIC: position independent code (does not depend on absolute addr. in memory)
clang $CFLAGS -o ./build/libplug.so -fPIC -shared ./src/plug.c $LIBS

clang $CFLAGS -DHOTRELOAD -o ./build/musicviz  ./src/musicviz.c $LIBS -L./build/ 
# clang $CFLAGS -DHOTRELOAD -o ./build/musicviz ./src/musicviz.c $LIBS -L./build/ -Wl,-rpath,'$ORIGIN/build'
# clang $CFLAGS -o ./build/musicviz ./src/plug.c ./src/musicviz.c $LIBS -L./build/ 
clang -o ./build/fft ./src/fft.c -lm

# Run the program with the specified arguments
# ./build/musicviz "$@"

