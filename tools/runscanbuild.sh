#!/bin/sh

CLANGDIR=/usr/bin

CHECKERS="\
    --use-analyzer=$CLANGDIR/clang \
    -disable-checker deadcode.DeadStores \
    -enable-checker alpha.core.CastSize \
    -enable-checker alpha.core.CastToStruct \
    -enable-checker alpha.core.IdenticalExpr \
    -enable-checker alpha.core.SizeofPtr \
    -enable-checker alpha.security.ArrayBoundV2 \
    -enable-checker alpha.security.MallocOverflow \
    -enable-checker alpha.security.ReturnPtrRange \
    -enable-checker alpha.unix.SimpleStream
    -enable-checker alpha.unix.cstring.BufferOverlap \
    -enable-checker alpha.unix.cstring.NotNullTerminated \
    -enable-checker alpha.unix.cstring.OutOfBounds \
    -enable-checker alpha.core.FixedAddr \
    -enable-checker security.insecureAPI.strcpy \
    -enable-checker security.FloatLoopCounter \
    -enable-checker security.insecureAPI.UncheckedReturn \
    -enable-checker security.insecureAPI.getpw \
    -enable-checker security.insecureAPI.gets \
    -enable-checker security.insecureAPI.mkstemp \
    -enable-checker security.insecureAPI.mktemp \
    -enable-checker security.insecureAPI.rand \
    -enable-checker cplusplus.NewDelete \
    -enable-checker unix.API \
    -enable-checker unix.Malloc \
    -enable-checker unix.MallocSizeof \
    -enable-checker unix.MismatchedDeallocator \
    -enable-checker unix.cstring.BadSizeArg \
    -enable-checker unix.cstring.NullArg
"

$CLANGDIR/scan-build \
    $CHECKERS \
    --use-analyzer=$CLANGDIR/clang \
    make -j2
