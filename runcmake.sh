#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
$DIR/cleancmake.sh

CXX=/usr/local/bin/clang++ CC=/usr/local/bin/clang cmake \
   -DCMAKE_BUILD_TYPE=debug \
   -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
   -DCMAKE_INSTALL_PREFIX=/usr \
   $DIR

