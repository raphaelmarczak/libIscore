#!/bin/sh

#make distclean
rm -f *~ core *.core
rm -f libIscore.1.0.0.dylib
rm -f libIscore.dylib libIscore.1.dylib libIscore.1.0.dylib libIscore.a

qmake -spec macx-g++
make
./install.sh
