#!/bin/bash
find . -name "CMakeFiles" -type d -exec rm -fr {} \;
rm -f CMakeCache.txt
rm -f Makefile
