#!/bin/bash

SOURCES=("core.c")
SOURCES_DIR=../sources/

BINARY=./core.out

OBJECTS_DIR=./

CC="gcc"
CC_FLAGS="-Wall -Wextra -Wpedantic -L../libs/testlib/build -ltestlib -I../libs/testlib/headers -L../libs/aiko/build-gcc-linux -laiko -I../libs/aiko/headers"

set -x

rm $OBJECTS_DIR/*.o -f

for SOURCE in $SOURCES; do
    $CC -c $SOURCES_DIR/$SOURCE -o "$OBJECTS_DIR/$(basename $SOURCE .c).o" $CC_FLAGS
done

$CC $OBJECTS_DIR/*.o -o $BINARY $CC_FLAGS

rm $OBJECTS_DIR/*.o -f
