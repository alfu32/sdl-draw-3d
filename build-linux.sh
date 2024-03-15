#!/bin/bash


make
rm -f build/linux64/voxd31
mv voxd31 build/linux64/
tar -czvf build/voxd31-x86_64-linux.tar.gz build/linux64