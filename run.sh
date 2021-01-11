#!/bin/bash

make
./build/exec/main
cp Image.ppm rendered/Image.ppm
convert Image.ppm rendered/Image.png
eog Image.ppm
