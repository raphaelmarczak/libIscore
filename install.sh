#!/bin/sh

if [ -x libIscore.so.1.0.0 ]
then
	sudo cp libIscore.a /usr/local/lib/
	sudo cp libIscore.so.1.0.0 /usr/local/lib/
	sudo ln -f -s /usr/lib/libIscore.so.1.0.0 /usr/lib/libIscore.so.1.0
	sudo ln -f -s /usr/lib/libIscore.so.1.0.0 /usr/lib/libIscore.so.1
	sudo ln -f -s /usr/lib/libIscore.so.1.0.0 /usr/lib/libIscore.so
	sudo ldconfig
fi
if [ -x libIscore.1.0.0.dylib ]
then
	sudo cp libIscore.1.0.0.dylib /usr/lib/

    sudo ln -f -s /usr/lib/libIscore.1.0.0.dylib /usr/lib/libIscore.1.0.dylib
	sudo ln -f -s /usr/lib/libIscore.1.0.0.dylib /usr/lib/libIscore.1.dylib
	sudo ln -f -s /usr/lib/libIscore.1.0.0.dylib /usr/lib/libIscore.dylib
fi
sudo mkdir -p /usr/local/include/IScore
sudo cp -R Includes/* /usr/local/include/IScore/

