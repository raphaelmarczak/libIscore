#!/bin/sh

make distclean
qmake -spec macx-g++
make
./install.sh
