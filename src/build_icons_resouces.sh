#!/bin/sh

echo Building embedded icons resouces started...

cd assets/icons

bin2c -C resources.icons.cpp -H resources.icons.h --wxbitmap *.png

mv resources.icons.cpp ../../resources/
mv resources.icons.h ../../resources/

cd ../..

echo Building embedded icons resouces finished.
