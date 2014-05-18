#!/bin/bash

BASEDIR=$(dirname $0)
cd $BASEDIR/../..
cppcheck --enable=all --inconclusive --std=posix -i build/ -i src/thirdparty/aqp . 2> cppcheck.txt

