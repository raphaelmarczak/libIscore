#!/bin/sh

sudo cp libIscore.a /usr/local/lib/
sudo cp libIscore.so* /usr/local/lib/
sudo mkdir -p /usr/local/include/IScore
sudo cp -R Includes/* /usr/local/include/IScore/
