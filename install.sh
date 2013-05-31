#!/bin/sh

if [ -a libIscore.so.1.0.0 ]
then
	sudo cp libIscore.a /usr/local/lib/
	sudo cp libIscore.so.1.0.0 /usr/local/lib/
	sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so.1.0
	sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so.1
	sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so
	sudo ldconfig
fi
if [ -a libIscore.1.0.0.dylib ]
then
	sudo cp libIscore.1.0.0.dylib /usr/local/lib/
#	sudo ln -s /usr/local/lib/libIscore.1.0.0.dylib /usr/local/lib/libIscore.1.0.dylib
	sudo ln -s /usr/local/lib/libIscore.1.0.0.dylib /usr/local/lib/libIscore.1.dylib
	sudo ln -s /usr/local/lib/libIscore.1.0.0.dylib /usr/local/lib/libIscore.dylib
fi
sudo mkdir -p /usr/local/include/IScore
sudo cp -R Includes/* /usr/local/include/IScore/

