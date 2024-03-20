#!/bin/sh

echo Hi there!

cd icons
bin2c -C resources.icons.cpp *
cd ..
