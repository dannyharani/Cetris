#!/usr/bin/env sh

set -xe

CFLAGS="-Wall -Wextra `pkg-config --cflags raylib`"
CLIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

cc $CFLAGS -o Cetris main.c $CLIBS