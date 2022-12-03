#!/bin/bash

SOURCES=("message_box.c kernel.c scheduler.c")
SOURCES_DIR=../sources/

OBJECTS_DIR=./

CC="gcc"
CC_FLAGS="-Wall -Wextra -Wpedantic -L../libs/testlib/build -ltestlib -I../libs/testlib/headers -L../libs/aiko/build-gcc-linux -laiko -I../libs/aiko/headers -Wno-unused-parameter"

set -x

rm $OBJECTS_DIR/*.out -f

for SOURCE in $SOURCES; do
    $CC $SOURCES_DIR/$SOURCE -o "$OBJECTS_DIR/$(basename $SOURCE .c).out" $CC_FLAGS
done
