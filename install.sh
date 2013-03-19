#!/bin/sh

sudo cp libIscore.a /usr/local/lib/
sudo cp libIscore.so.1.0.0 /usr/local/lib/
sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so.1.0
sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so.1
sudo ln -s /usr/local/lib/libIscore.so.1.0.0 /usr/local/lib/libIscore.so
sudo ldconfig
sudo mkdir -p /usr/local/include/IScore
sudo cp -R Includes/* /usr/local/include/IScore/
