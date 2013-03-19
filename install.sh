#!/bin/sh

sudo cp libIscore.a /usr/local/lib/
if [ -x libIscore.so.1.0.0 ]
then
	sudo cp libIscore.so.1.0.0 /usr/local/lib/
	sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so.1.0
	sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so.1
	sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so
fi
if [ -x libIscore.dylib.1.0.0 ]
then
	sudo cp libIscore.dylib.1.0.0 /usr/local/lib/
	sudo ln -s /usr/local/lib/libIscore.dylib.1.0.0 /usr/local/lib/libIscore.dylib.1.0
	sudo ln -s /usr/local/lib/libIscore.dylib.1.0.0 /usr/local/lib/libIscore.dylib.1
	sudo ln -s /usr/local/lib/libIscore.dylib.1.0.0 /usr/local/lib/libIscore.dylib
fi
sudo ldconfig
sudo mkdir -p /usr/local/include/IScore
sudo cp -R Includes/* /usr/local/include/IScore/

