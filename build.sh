set -xe


CFLAGS="-Wall -Wextra -ggdb `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib` -lglfw -lm -ldl -lpthread"

cp -r ./shaders ./build
mkdir -p ./build
# -fPIC: position independent code (does not depend on absolute addr. in memory)

#if [ ! -z "${HOTRELOAD}" ]; then
#  echo "*************************************************"
#  gcc $CFLAGS -o ./build/libplug.so -fPIC -shared ./src/plug.c $LIBS
#  gcc $CFLAGS -DHOTRELOAD -o ./build/musicviz ./src/musicviz.c $LIBS -L./build/ -Wl,-rpath=./build
#
#else
#
#  echo "========================================================"
#  clang $CFLAGS  -o ./build/musicviz ./src/plug.c ./src/musicviz.c $LIBS -L./build/
#fi


gcc $CFLAGS -o ./build/libplug.so -fPIC -shared ./src/plug.c $LIBS
gcc $CFLAGS -DHOTRELOAD -o ./build/musicviz ./src/musicviz.c $LIBS -L./build/ -Wl,-rpath=./build
# gcc $CFLAGS -DHOTRELOAD -o ./build/musicviz ./src/musicviz.c $LIBS -L./build/ 
# -lplug -Wl,-rpath,./build


clang -o ./build/fft ./src/fft.c -lm

